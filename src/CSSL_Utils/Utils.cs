using ArcGIS.Core.CIM;
using ArcGIS.Core.Data;
using ArcGIS.Core.Data.Raster;
using ArcGIS.Core.Data.UtilityNetwork.Trace;
using ArcGIS.Core.Geometry;
using ArcGIS.Core.Internal.Geometry;
using ArcGIS.Desktop.Mapping;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Windows;

namespace CSSL_ArcGISPro_Utils
{
    public class Utils
    {
        [DllImport("kernel32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        static extern bool AllocConsole();

        public static void OpenConsole()
        {
            AllocConsole();

            var stdout = Console.OpenStandardOutput();
            var writer = new StreamWriter(stdout) { AutoFlush = true };
            Console.SetOut(writer);
            Console.SetError(TextWriter.Null);
        }

        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern IntPtr LoadLibrary(string dllToLoad);

        public static int LoadDll(string path)
        {
            IntPtr hModule = LoadLibrary(path);

            if (hModule == IntPtr.Zero) return Marshal.GetLastWin32Error();

            return 0;
        }

        public static string AddinAssemblyLocation()
        {
            var asm = System.Reflection.Assembly.GetExecutingAssembly();

            return Path.GetDirectoryName(Uri.UnescapeDataString(new Uri(asm.Location).LocalPath));
        }

        public static string GetProjectPath()
        {
            return Path.GetDirectoryName(ArcGIS.Desktop.Core.Project.Current.URI);
        }

        public static IEnumerable<RasterLayer> GetRasterLayers()
        {
            var map = MapView.Active.Map;

            return map.GetLayersAsFlattenedList().OfType<RasterLayer>();
        }

        public static RasterDataset OpenRasterDataset(string directory, string name)
        {
            RasterDataset rasterDatasetToOpen = null;

            try
            {
                FileSystemConnectionPath connectionPath = new(new Uri(directory), FileSystemDatastoreType.Raster);

                FileSystemDatastore dataStore = new(connectionPath);

                rasterDatasetToOpen = dataStore.OpenDataset<RasterDataset>(name);
            }
            catch (Exception) {}

            return rasterDatasetToOpen;
        }

        public static RasterLayer LoadRasterLayer(string directory, string name, ILayerContainerEdit group=null)
        {
            var rasterDataset = OpenRasterDataset(directory, name);
            var rasterLayerCreationParams = new RasterLayerCreationParams(rasterDataset);

            group ??= MapView.Active.Map;

            return LayerFactory.Instance.CreateLayer<RasterLayer>(rasterLayerCreationParams, group);
        }

        public static FeatureLayer LoadShapeFile(string directory, string name, ILayerContainerEdit group = null)
        {
            group ??= MapView.Active.Map;

            try
            {
                FileSystemConnectionPath connectionPath = new(new Uri(directory), FileSystemDatastoreType.Shapefile);
                FileSystemDatastore dataStore = new(connectionPath);
                FeatureClass featureClass = dataStore.OpenDataset<FeatureClass>(name);

                var layerParams = new FeatureLayerCreationParams(featureClass);

                return LayerFactory.Instance.CreateLayer<FeatureLayer>(layerParams, group);
            }
            catch (Exception) { }

            return null;
        }

        public static RasterDataset DuplicateRasterDataset(string inputFolder, string inputName, string outputFolder, string outputName, 
                                                           RasterCompressionType compressionType=RasterCompressionType.LZW, String fileType="TIFF")
        {
            RasterDataset inRd = OpenRasterDataset(inputFolder, inputName);
            Raster inRaster = inRd.CreateFullRaster();

            FileSystemConnectionPath outputConnectionPath = new(new Uri(outputFolder), FileSystemDatastoreType.Raster);

            FileSystemDatastore outputFileSytemDataStore = new(outputConnectionPath);

            RasterStorageDef rasterStorageDef = new();
            rasterStorageDef.SetPyramidLevel(0);
            rasterStorageDef.SetCompressionType(compressionType);

            inRaster.SaveAs(outputName, outputFileSytemDataStore, fileType, rasterStorageDef);

            return OpenRasterDataset(outputFolder, outputName);
        }

        public static bool HasDataStore(FeatureLayer layer)
        {
            using FeatureClass featureClass = layer.GetFeatureClass();
            using Datastore datastore = featureClass.GetDatastore();

            // check if layer has a valid datastore
            if (!(datastore is FileSystemDatastore || datastore is Geodatabase)) return false;

            //check that the layer is not empty
            return featureClass.GetCount() > 0;
        }

        private static readonly Dictionary<Type, esriGeometryType> geometryDict = new() { { typeof(MapPoint), esriGeometryType.esriGeometryPoint },
                                                                                          { typeof(Polyline), esriGeometryType.esriGeometryPolyline },
                                                                                          { typeof(Polygon), esriGeometryType.esriGeometryPolygon }, };

        public static bool IsFeatureLayerOfType<T>(FeatureLayer layer) where T : Geometry
        {
            if (!HasDataStore(layer)) return false;

            return geometryDict.ContainsKey(typeof(T)) && geometryDict[typeof(T)] == layer.ShapeType;
        }

        public static List<T> ReadShapes<T>(FeatureLayer layer) where T : Geometry
        {
            List<T> list = [];

            using RowCursor rowCursor = layer.GetTable().Search();

            while (rowCursor.MoveNext())
            {
                Geometry shape = ((Feature)rowCursor.Current).GetShape();
                list.Add((T)shape);
            }

            return list;
        }


    }
}
