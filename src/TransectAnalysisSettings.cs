using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.Json.Serialization;
using System.Threading.Tasks;

namespace TreefallPatternAnalysis
{
    public class TransectAnalysisSettings
    {
        [JsonInclude]
        public (double, double) vrRange;
        [JsonInclude]
        public (double, double) vtRange;
        [JsonInclude]
        public (double, double) vsRange;
        [JsonInclude]
        public (double, double) vcRange;
        [JsonInclude]
        public double[] selectedModels;
        [JsonInclude]
        public double threshold;
        [JsonInclude]
        public int numOfSimulations;
        [JsonInclude]
        public int patternType;
        [JsonInclude]
        public bool useGustVel;
        [JsonInclude]
        public bool randomizeTransect;

        public TransectAnalysisSettings() {
            selectedModels = [0, 1, 2, 3, 4, 5];
            vrRange = (20, 80);
            vtRange = (1, 50);
            vsRange = (10, 20);
            vcRange = (30, 40);
            threshold = 0.1;
            numOfSimulations = 10000;
            patternType = 0;
            useGustVel = true;
            randomizeTransect = true;
        }
    }
}
