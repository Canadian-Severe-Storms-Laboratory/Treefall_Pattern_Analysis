using ArcGIS.Core.Data;
using ArcGIS.Core.Data.Raster;
using ArcGIS.Core.Geometry;
using ArcGIS.Desktop.Mapping;
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace ArcGISUtils
{
    internal class Utils
    {
        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern IntPtr LoadLibrary(string dllToLoad);

        private static bool LoadDll(string path)
        {
            IntPtr hModule = LoadLibrary(path);

            if (hModule == IntPtr.Zero)
            {
                int errorCode = Marshal.GetLastWin32Error();
                System.Windows.MessageBox.Show("Failed to load DLLs: " + errorCode.ToString());
                return false;
            }

            return true;
        }

        public static bool PreLoadDlls()
        {
            string path;

            #if DEBUG
                path = @"C:\Users\danie\Documents\Treefall Pattern Analysis\TreefallPatternAnalysisV2\bin\Debug\net8.0-windows";
            #else
                path = AddinAssemblyLocation();
            #endif

            string[] dlls = ["PatternSolver"];

            foreach (string dll in dlls)
            {
                if (!LoadDll(path + "\\" + dll + ".dll")) return false;
            }

            return true;
        }

        public static string AddinAssemblyLocation()
        {
            var asm = System.Reflection.Assembly.GetExecutingAssembly();

            return System.IO.Path.GetDirectoryName(Uri.UnescapeDataString(new Uri(asm.Location).LocalPath));
        }

        public static string GetProjectPath()
        {
            return System.IO.Path.GetDirectoryName(ArcGIS.Desktop.Core.Project.Current.URI);
        }

        public static RasterLayer LoadRasterLayer(string directory, string name)
        {
            var rasterDataset = OpenRasterDataset(directory, name);
            var rasterLayerCreationParams = new RasterLayerCreationParams(rasterDataset);
            
            return LayerFactory.Instance.CreateLayer<RasterLayer>(rasterLayerCreationParams, MapView.Active.Map);

            //return LayerFactory.Instance.CreateLayer(new Uri(path), MapView.Active.Map) as RasterLayer;
        }

        public static Tuple<int, int, int, int> EnvolopeToImageRect(Envelope envelope, Raster raster)
        {
            var topLeft = raster.MapToPixel(envelope.XMin, envelope.YMax);
            var bottomRight = raster.MapToPixel(envelope.XMax, envelope.YMin);

            int x = Math.Min(topLeft.Item1, bottomRight.Item1);
            int y = Math.Min(topLeft.Item2, bottomRight.Item2);
            int width = Math.Abs(bottomRight.Item1 - topLeft.Item1);
            int height = Math.Abs(topLeft.Item2 - bottomRight.Item2);

            return new Tuple<int, int, int, int>(x, y, width, height);
        }

        public static RasterDataset OpenRasterDataset(string folder, string name)
        {
            // Create a new raster dataset which is set to null
            RasterDataset rasterDatasetToOpen = null;
            try
            {
                // Create a new file system connection path to open raster datasets using the folder path.
                FileSystemConnectionPath connectionPath = new FileSystemConnectionPath(new System.Uri(folder), FileSystemDatastoreType.Raster);
                // Create a new file system data store for the connection path created above.
                FileSystemDatastore dataStore = new FileSystemDatastore(connectionPath);
                // Open the raster dataset.
                rasterDatasetToOpen = dataStore.OpenDataset<RasterDataset>(name);
                // Check if it is not null. If it is show a message box with the appropriate message.
                if (rasterDatasetToOpen == null)
                    System.Windows.MessageBox.Show("Failed to open raster dataset: " + name);
            }
            catch (Exception exc)
            {
                // If an exception occurs, show a message box with the appropriate message.
                System.Windows.MessageBox.Show("Exception caught in OpenRasterDataset for raster: " + name + exc.Message);
            }
            return rasterDatasetToOpen;
        }
    
        public static bool IsShapeFile(FeatureLayer layer)
        {
            using FeatureClass featureClass = layer.GetFeatureClass();
            using Datastore datastore = featureClass.GetDatastore();
            if (datastore is FileSystemDatastore)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        public static bool IsShapeFileOfType<T>(FeatureLayer layer) where T : Geometry
        {
            if (!IsShapeFile(layer)) return false;

            string layertype = layer.ShapeType.ToString().Substring(12).ToLower();
            var splitShapeType = typeof(T).ToString().Split(".");
            string shapetype = splitShapeType[splitShapeType.Length - 1].ToLower();

            if (layertype != shapetype) return false;

            return true;
        }
        

        public static List<T> ReadShapes<T>(FeatureLayer layer) where T : Geometry
        {
            List<T> list = new List<T>();
            using RowCursor rowCursor = layer.GetTable().Search();

            while (rowCursor.MoveNext())
            {
                Geometry shape = ((Feature)rowCursor.Current).GetShape();
                list.Add((T)shape);
            }

            return list;
        }

        public static List<double> ReadVectors(FeatureLayer layer)
        {
            List<double> vecs = [];

            using Table shp_table = layer.GetTable();
            using RowCursor rowCursor = shp_table.Search();

            while (rowCursor.MoveNext())
            {
                using Feature f = (Feature)rowCursor.Current;

                Polyline pl = (Polyline)f.GetShape();

                vecs.AddRange([pl.Points[0].X, pl.Points[0].Y, pl.Points[1].X, pl.Points[1].Y]);
            }

            return vecs;
        }

        public static List<double> ReadPolyline(FeatureLayer layer)
        {
            List<double> polyline = [];

            using Table shp_table = layer.GetTable();
            using RowCursor rowCursor = shp_table.Search();

            if (!rowCursor.MoveNext()) return polyline; 

            using Feature f = (Feature)rowCursor.Current;

            Polyline pl = (Polyline)f.GetShape();

            polyline = new(pl.PointCount * 2);

            foreach (var p in pl.Points)
            {
                polyline.AddRange([p.X, p.Y]);
            }
            
            return polyline;
        }

        public static double RoundUp(double value, int decimalPlaces)
        {
            double multiplier = Math.Pow(10, decimalPlaces);
            return Math.Ceiling(value * multiplier) / multiplier;
        }

        public static double RoundDown(double value, int decimalPlaces)
        {
            double multiplier = Math.Pow(10, decimalPlaces);
            return Math.Floor(value * multiplier) / multiplier;
        }

    }
}
