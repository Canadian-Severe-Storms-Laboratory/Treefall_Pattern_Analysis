using ArcGIS.Desktop.Mapping;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Controls;


namespace TreefallPatternAnalysis
{
    /// <summary>
    /// Interaction logic for RasterSelectionList.xaml
    /// </summary>
    public partial class RasterSelectionList : UserControl
    {
        private IEnumerable<RasterLayer> rLayers;

        public RasterSelectionList()
        {
            InitializeComponent();

            //get current map
            var map = MapView.Active.Map;

            //get all raster layers on map
            rLayers = map.GetLayersAsFlattenedList().OfType<RasterLayer>();

            foreach (var layer in rLayers)
            {
                listBox.Items.Add(layer.Name);
            }
        }

        public List<RasterLayer> GetSelectedLayers()
        {
   
            List<RasterLayer> selectedRasters = new List<RasterLayer>();

            foreach (string name in listBox.SelectedItems)
            {
                foreach (var raster in rLayers)
                {
                    if (raster.Name.Equals(name))
                    {
                        selectedRasters.Add(raster);
                    }
                }
            }
            
            return selectedRasters;
        }

        public bool IsEmpty()
        {
            return listBox.Items.Count == 0;
        }
    }
}
