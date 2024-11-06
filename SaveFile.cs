using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.Text.Json;
using System.Windows;

namespace TreefallPatternAnalysis
{
    static class SaveFile
    {
        public static List<Transect> LoadSaveFile(string filePath, RasterSelectionBox vectorBox, RasterSelectionBox lineBox)
        {
            if (!System.IO.File.Exists(filePath)) 
            {
                MessageBox.Show("File does not exist");
                return null;
            }

            string save = System.IO.File.ReadAllText(filePath);

            string[] data = save.Split(new string[] { ";\n" }, 3, StringSplitOptions.None);

            if(!vectorBox.SelectByName(data[0]))
            {
                MessageBox.Show("Could not find vectors shapefile layer");
                return null;
            }

            if (!lineBox.SelectByName(data[1]))
            {
                MessageBox.Show("Could not find convergence line shapefile layer");
                return null;
            }

            var transects = JsonSerializer.Deserialize<List<Transect>>(data[2]);

            return transects;
        }

        public static void WriteSaveFile(List<Transect> transects, string[] layerNames)
        {
            SaveFileDialog saveFileDialog = new()
            {
                Filter = "JSON|*.json",
                Title = "Save Treefall Pattern Analysis"
            };
                
            if(saveFileDialog.ShowDialog() == true)
            {
                JsonSerializerOptions options = new JsonSerializerOptions
                {
                    WriteIndented = true
                };

                string save = layerNames[0] + ";\n" +
                              layerNames[1] + ";\n" +
                              JsonSerializer.Serialize(transects, options);

                System.IO.File.WriteAllText(saveFileDialog.FileName, save);
            }          
        }
    }
}
