using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.Linq;
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

            string[] data = save.Split(new string[] { ";\n" }, 4, StringSplitOptions.None);

            if (data.Length < 3)
            {
                MessageBox.Show("Save file is corrupted or incomplete");
                return null;
            }

            if (!vectorBox.SelectByName(data[0]))
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

            if (data.Length < 4)
            {
                for (int i = 0; i < transects.Count; i++)
                {
                    transects[i].analysisSettings = new();
                }
                return transects;
            }

            JsonSerializerOptions options = new JsonSerializerOptions
            {
                IncludeFields = true,
            };

            var settings = JsonSerializer.Deserialize<List<TransectAnalysisSettings>>(data[3], options);

            if (transects.Count != settings.Count)
            {
                MessageBox.Show("Save file is corrupted or incomplete");
                return null;
            }

            for (int i = 0; i < transects.Count; i++)
            {
                transects[i].analysisSettings = settings[i];
            }

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
                JsonSerializerOptions tOptions = new JsonSerializerOptions
                {
                    WriteIndented = true
                };

                JsonSerializerOptions sOptions = new JsonSerializerOptions
                {
                    IncludeFields = true,
                    WriteIndented = true
                };

                List<TransectAnalysisSettings> settings = transects.Select(t => t.analysisSettings).ToList();

                string save = layerNames[0] + ";\n" +
                              layerNames[1] + ";\n" +
                              JsonSerializer.Serialize(transects, tOptions) + ";\n" +
                              JsonSerializer.Serialize(settings, sOptions);

                System.IO.File.WriteAllText(saveFileDialog.FileName, save);
            }          
        }
    }
}
