using ArcGIS.Core.Geometry;
using ArcGIS.Desktop.Framework.Threading.Tasks;
using ArcGIS.Desktop.Mapping;
using System;
using System.Collections.Generic;
using System.Threading.Tasks;
using System.Windows;
using static ArcGISUtils.Utils;
using System.Runtime.InteropServices;
using System.Windows.Controls;
using ScottPlot;
using ArcGIS.Desktop.Internal.Mapping;
using System.Linq;

namespace TreefallPatternAnalysis
{
    public partial class MainPatternAnalysis : ArcGIS.Desktop.Framework.Controls.ProWindow
    {
        [DllImport("kernel32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        static extern bool AllocConsole();

        public MainPatternAnalysis()
        {
            InitializeComponent();
            
            AllocConsole();

            vmaxHistogramPlot.Plot.Style(figureBackground: System.Drawing.Color.FromArgb(255, 229, 229, 229));
            swirlHistogramPlot.Plot.Style(figureBackground: System.Drawing.Color.FromArgb(255, 229, 229, 229));

            vmaxHistogramPlot.Plot.Layout(left: 0, right: 0, bottom: 0, top: 0);
            swirlHistogramPlot.Plot.Layout(left: 0, right: 0, bottom: 0, top: 0);

            vmaxHistogramPlot.Plot.Title("Estimated Vmax");
            swirlHistogramPlot.Plot.Title("Estimated Swirl Ratio");
        }

        private bool loaded = false;

        private void ProWindow_Loaded(object sender, RoutedEventArgs e)
        {
            loaded = true;
        }

        private void ModelTypeListView_Loaded(object sender, RoutedEventArgs e)
        {
            modelTypeListView.SelectAll();
        }

        private async Task<Tuple<FeatureLayer, FeatureLayer>> GetInputPolylineFiles()
        {
            // no selection
            if (VectorsSelectionBox.IsEmpty() || ConvergenceSelectionBox.IsEmpty()) return null;

            FeatureLayer selectedVector = VectorsSelectionBox.GetSelectedLayer();
            FeatureLayer selectedConvergence = ConvergenceSelectionBox.GetSelectedLayer();

            // not feature layers or files unreadable
            if (selectedVector == null || selectedConvergence == null) return null;

            bool arePolylineFiles = await QueuedTask.Run(() =>
            {
                if (IsShapeFileOfType<Polyline>(selectedVector) && IsShapeFileOfType<Polyline>(selectedConvergence)) return true;

                return false;
            });

            // not polyline shapefiles
            if (!arePolylineFiles) return null;

            return new(selectedVector, selectedConvergence);
        }

        private VecHashGrid vecHashGrid;
        private ConvergenceLine convergenceLine;

        private async void InputSelectionNextButton(object sender, RoutedEventArgs e)
        {
            await InputSelection();
        }

        private async Task InputSelection()
        {
            try
            {
                var (selectedVector, selectedConvergence) = (await GetInputPolylineFiles()) ?? throw new Exception();

                await QueuedTask.Run(() =>
                {
                    List<double> overviewVectors = ReadVectors(selectedVector);

                    vecHashGrid = new VecHashGrid(overviewVectors.ToArray());

                    List<double> convergenceLinePoints = ReadPolyline(selectedConvergence);

                    convergenceLine = new ConvergenceLine(convergenceLinePoints.ToArray());
                });

                transectPositionSlider.Maximum = Math.Floor(convergenceLine.length());
                transectPositionSlider.TickFrequency = convergenceLine.length() / 10;

                headerTabs.SelectedIndex = 1;

                transectPlot.DrawOverviewPlot(vecHashGrid, convergenceLine);
            }
            catch (Exception ex)
            {
                MessageBox.Show("Failed to load specified Files, the files must be in a\n" +
                                "polyline shapefile where each vector is a polyline, and the\n" +
                                "convergence line must be a single polyline\n\nError:\n\n" + ex.Message);
            }
        }

        private async void LoadSave(object sender, RoutedEventArgs e)
        {
            var transects = SaveFile.LoadSaveFile(saveFileSelector.GetFilePath(), VectorsSelectionBox, ConvergenceSelectionBox);

            if (transects == null) return;

            await InputSelection();

            if (headerTabs.SelectedIndex != 1) return;

            transectCreationList.LoadFromSave(transects, transectPlot.OverviewPlot());
        }

        private void ClearPlots()
        {
            ObservedPatternPlot.RemoveObsPattern();
            ObservedPatternPlot.RemoveSimPattern();
            transectPlot.UpdateVectorsPlot([]);
            transectPlot.RefreshPlots();
        }

        private bool lockSliders = false;
        private void TransectChanged(object sender, RoutedEventArgs e)
        {
            if (!loaded || lockSliders) return;

            if (transectCreationList.SelectedTransect() == null)
            {
                ClearPlots();
                return;
            }

            lockSliders = true;

            Transect selectedTransect = transectCreationList.SelectedTransect();

            bool initalized = selectedTransect.intialized;

            if (!initalized || sender is SliderAndTextBox || sender is NumberBox)
            {
                double[] pt = convergenceLine.query(transectPositionSlider.GetValue());

                selectedTransect.spacing = vectorSpacing.GetNumber();
                selectedTransect.positionOffset = transectPositionSlider.GetValue();
                selectedTransect.heightOffset = transectHeightOffsetSlider.GetValue();
                selectedTransect.angleOffset = transectAngleOffsetSlider.GetValue();
                selectedTransect.lengthAbove = transectLengthAboveSlider.GetValue();
                selectedTransect.lengthBelow = transectLengthBelowSlider.GetValue();
                selectedTransect.width = transectWidthSlider.GetValue();
                selectedTransect.x = pt[0];
                selectedTransect.y = pt[1];
                selectedTransect.setPerpendicularAngle(pt[2]);
            }
            else if (sender is TransectCreationList)
            {
                vectorSpacing.SetNumber(selectedTransect.spacing);
                transectPositionSlider.SetValue(selectedTransect.positionOffset);
                transectHeightOffsetSlider.SetValue(selectedTransect.heightOffset);
                transectAngleOffsetSlider.SetValue(selectedTransect.angleOffset);
                transectLengthAboveSlider.SetValue(selectedTransect.lengthAbove);
                transectLengthBelowSlider.SetValue(selectedTransect.lengthBelow);
                transectWidthSlider.SetValue(selectedTransect.width);
            }

            ObservedPatternPlot.RemoveSimPattern();

            if (!initalized) selectedTransect.Render(transectPlot.OverviewPlot());
            else selectedTransect.Update();

            double[] transectVectors = vecHashGrid.query(selectedTransect);

            transectPlot.UpdateVectorsPlot(transectVectors);
            transectPlot.RefreshPlots();

            if (vectorSpacing.GetNumber() > 0.0)
            {
                var pattern = new ObservedPattern(selectedTransect.lengthAbove, selectedTransect.lengthBelow, selectedTransect.spacing, transectVectors);

                ObservedPatternPlot.Display(pattern);

                validTransectSignal.Fill = pattern.isValid() ? new System.Windows.Media.SolidColorBrush(System.Windows.Media.Colors.Green) : new System.Windows.Media.SolidColorBrush(System.Windows.Media.Colors.Red);
            }

            PlotTransectResults(null, null);

            lockSliders = false;
        }

        private async void AutoFitTransects(object sender, RoutedEventArgs e)
        { 
            autoFitTransectsButton.IsEnabled = false;
            AutoTransectFitter atf = new(vecHashGrid, convergenceLine);

            atf.lengthBelowRange.min = transectLengthBelowSlider.GetValue() * 0.75;
            atf.lengthBelowRange.max = transectLengthBelowSlider.GetValue() * 1.25;
            atf.lengthAboveRange.min = transectLengthAboveSlider.GetValue() * 0.75;
            atf.lengthAboveRange.max = transectLengthAboveSlider.GetValue() * 1.25;
            atf.spacingRange.min = vectorSpacing.GetNumber() * 0.75;
            atf.spacingRange.max = vectorSpacing.GetNumber() * 1.25;
            atf.matchThreshold = cutoffThreshold.GetNumber() / 2.0;

            Monitor monitor = atf.monitor;
            monitor.Start();

            await QueuedTask.Run(atf.randomClusterSearch);

            monitor.Stop();

            if (monitor.cancelled)
            {
                autoFitTransectsButton.IsEnabled = true;
                return;
            }
            if (atf.failed)
            {
                MessageBox.Show("Failed to find a transect");
                autoFitTransectsButton.IsEnabled = true;
                return;
            }

            int numFound = atf.resultsSize();

            List<Transect> transects = [];

            for (int i = 0; i < numFound; i++)
            {
                transects.Add(atf.getResult(i));
            }

            transects.Sort((t1, t2) => t1.positionOffset.CompareTo(t2.positionOffset));

            for (int i = 0; i < numFound; i++)
            {
                transects[i].Render(transectPlot.OverviewPlot());
                transectCreationList.Add(transects[i]);
            }

            autoFitTransectsButton.IsEnabled = true;
        }

        private void NextTab(object sender, RoutedEventArgs e)
        {
            headerTabs.SelectedIndex++;
        }

        private void FineAdjustment(object sender, RoutedEventArgs e)
        {
            if (sender == null || sender is not Button) return;

            switch((sender as Button).Tag)
            {
                case "up":
                    transectHeightOffsetSlider.SetValue(transectHeightOffsetSlider.GetValue() + 1);
                    break;

                case "down":
                    transectHeightOffsetSlider.SetValue(transectHeightOffsetSlider.GetValue() - 1);
                    break;

                case "cw":
                    transectAngleOffsetSlider.SetValue(transectAngleOffsetSlider.GetValue() - 0.5);
                    break;

                case "ccw":
                    transectAngleOffsetSlider.SetValue(transectAngleOffsetSlider.GetValue() + 0.5);
                    break;

                default:
                    break;
            }
        }

        private ObservedPattern GetSelectedObservedPattern()
        {
            Transect selectedTransect = transectCreationList.SelectedTransect();
            double[] transectVectors = vecHashGrid.query(selectedTransect);
            ObservedPattern obsPattern = new(selectedTransect.lengthAbove, selectedTransect.lengthBelow, selectedTransect.spacing, transectVectors);

            return obsPattern;
        }

        private void TestMatch(object sender, RoutedEventArgs e)
        {
            if (transectCreationList.SelectedTransect() == null) return;

            Range VrRange = new() { min = vrmin.GetNumber(), max = vrmax.GetNumber() };
            Range VtRange = new() { min = vtmin.GetNumber(), max = vtmax.GetNumber() };
            Range VsRange = new() { min = vsmin.GetNumber(), max = vsmax.GetNumber() };
            Range VcRange = new() { min = vcmin.GetNumber(), max = vcmax.GetNumber() };

            PatternMatcher matcher = new (VrRange, VtRange, VsRange, VcRange);

            ObservedPattern obsPattern = GetSelectedObservedPattern();

            transectCreationList.SelectedTransect().bestMatchError = matcher.bestMatchError(obsPattern);

            Pattern simPattern = matcher.bestMatch(obsPattern);

            if (simPattern.vecs.Count == 0)
            {
                MessageBox.Show("No pattern match found");
                return;
            }

            ObservedPatternPlot.simScale = matcher.bestMatchScale;
            ObservedPatternPlot.Display(simPattern);
        }

        private int previousTab = -1;
        private bool lockTabs = false;
        private void TabChanged(object sender, SelectionChangedEventArgs e)
        {
            if (lockTabs) return;
            lockTabs = true;

            if ((headerTabs.SelectedItem as TabItem).Header.ToString() == "Save")
            {
                if (!transectCreationList.Transects().IsNullOrEmpty() && !VectorsSelectionBox.IsEmpty() && !ConvergenceSelectionBox.IsEmpty())
                {
                    SaveFile.WriteSaveFile(transectCreationList.Transects(), [VectorsSelectionBox.GetSelectedLayer().Name, ConvergenceSelectionBox.GetSelectedLayer().Name]);
                }

                headerTabs.SelectedIndex = previousTab;
                lockTabs = false;
                return;
            }

            transectGrid.Visibility = headerTabs.SelectedIndex == 0 ? Visibility.Hidden : Visibility.Visible;
            previousTab = headerTabs.SelectedIndex;
            lockTabs = false;
        }

        private async void RunSimulation(object sender, RoutedEventArgs e)
        {
            if (transectCreationList.SelectedTransect() == null) return;
            Transect selectedTransect = transectCreationList.SelectedTransect();

            Range VrRange = new() { min = vrmin.GetNumber(), max = vrmax.GetNumber() };
            Range VtRange = new() { min = vtmin.GetNumber(), max = vtmax.GetNumber() };
            Range VsRange = new() { min = vsmin.GetNumber(), max = vsmax.GetNumber() };
            Range VcRange = new() { min = vcmin.GetNumber(), max = vcmax.GetNumber() };

            PatternMatcher matcher = new(VrRange, VtRange, VsRange, VcRange)
            {
                numSimulations = numberOfSimulations.GetIntNumber(),
                matchThreshold = cutoffThreshold.GetNumber()
            };

            ObservedPattern obsPattern = GetSelectedObservedPattern();

            double error = matcher.bestMatchError(obsPattern);

            Pattern simPattern = matcher.bestMatch(obsPattern);

            if (simPattern.vecs.Count == 0)
            {
                MessageBox.Show("No pattern match found");
                return;
            }

            ObservedPatternPlot.simScale = matcher.bestMatchScale;
            ObservedPatternPlot.Display(simPattern);

            Monitor monitor = matcher.monitor;

            if (error > cutoffThreshold.GetNumber() / 2.0)
            {
                monitor.warning = "Warning: Pattern does not match very well";
            }

            selectedTransect.bestMatchError = error;

            monitor.Start();

            await QueuedTask.Run(() => matcher.monteCarloMatching(obsPattern));

            monitor.Stop();

            if (monitor.cancelled) return;

            selectedTransect.vmaxResults = matcher.minVelResults;
            selectedTransect.swirlResults = matcher.bestSwirlResults;

            PlotTransectResults(null, null);
        }

        private void PlotTransectResults(object sender, RoutedEventArgs e)
        {
            if (transectCreationList.SelectedTransect() == null) return;

            Transect transect = transectCreationList.SelectedTransect();

            if (transect.vmaxResults.IsNullOrEmpty() || transect.swirlResults.IsNullOrEmpty())
            {
                vmaxHistogramPlot.Plot.Clear();
                swirlHistogramPlot.Plot.Clear();
                vmaxHistogramPlot.Refresh();
                swirlHistogramPlot.Refresh();
                return;
            }

            PlotHist(vmaxHistogramPlot, transect.vmaxResults, (int)numVmaxHistBoxes.GetValue());
            PlotHist(swirlHistogramPlot, transect.swirlResults, (int)numSwirlHistBoxes.GetValue());
            WriteStats(transect);
        }

        private void PlotHist(WpfPlot plot, double[] data, int numBins)
        {
            Plot plt = plot.Plot;
            var stats = new ScottPlot.Statistics.BasicStats(data);

            plt.Clear();

            var hist = ScottPlot.Statistics.Histogram.WithFixedBinCount(stats.Min, stats.Max, numBins);

            hist.AddRange(data);

            var bar = plt.AddBar(hist.GetProbability(), positions: hist.Bins);
            bar.BarWidth = (stats.Max - stats.Min) / numBins;

            var medianLine = plt.AddVerticalLine(data[data.Length / 2], System.Drawing.Color.Black, 2, LineStyle.Dash);
            medianLine.PositionLabel = false;

            var dist = hist.GetProbabilityCurve(data, true);

            plt.AddFunction(dist, System.Drawing.Color.Black, 2, LineStyle.Solid);

            plt.SetAxisLimitsY(-0.01, Math.Max((double)dist(stats.Mean), hist.GetProbability().Max()) * 1.1);

            plot.Refresh();
        }

        private void WriteStats(Transect transect)
        {
            if (transect.vmaxResults.IsNullOrEmpty() || transect.swirlResults.IsNullOrEmpty())
            {
                resultStatsText.Text = "";
                return;
            }

            var vmaxStats = new ScottPlot.Statistics.BasicStats(transect.vmaxResults);
            var swirlStats = new ScottPlot.Statistics.BasicStats(transect.swirlResults);

            double vmaxMedian = transect.vmaxResults[transect.vmaxResults.Length / 2];
            double swirlMedian = transect.swirlResults[transect.swirlResults.Length / 2];

            double[] y = new double[transect.vmaxResults.Length];

            for (int i = 0; i < transect.vmaxResults.Length; i++)
            {
                y[i] = i / (double)transect.vmaxResults.Length;
            }

            double ef0 = Math.Round(y[FindClosestIndex(transect.vmaxResults, 37.0)] - y[FindClosestIndex(transect.vmaxResults, 25.0)], 2);
            double ef1 = Math.Round(y[FindClosestIndex(transect.vmaxResults, 50.0)] - y[FindClosestIndex(transect.vmaxResults, 37.0)], 2);
            double ef2 = Math.Round(y[FindClosestIndex(transect.vmaxResults, 62.0)] - y[FindClosestIndex(transect.vmaxResults, 50.0)], 2);
            double ef3 = Math.Round(y[FindClosestIndex(transect.vmaxResults, 75.0)] - y[FindClosestIndex(transect.vmaxResults, 62.0)], 2);
            double ef4 = Math.Round(y[FindClosestIndex(transect.vmaxResults, 87.0)] - y[FindClosestIndex(transect.vmaxResults, 75.0)], 2);
            double ef5 = Math.Round(1.0 - y[FindClosestIndex(transect.vmaxResults, 87.0)], 2);

            resultStatsText.Text = $"Error:\t{Math.Round(transect.bestMatchError, 4)}\n" +
                                   $"Vmax:\t{Math.Round(vmaxMedian)} ± {Math.Round(vmaxStats.StDev)}\n" +
                                   $"Swirl:\t{Math.Round(swirlMedian)} ± {Math.Round(swirlStats.StDev)}\n" +
                                   $"\nProbabilities\n" +
                                   $"EF0:\t{ef0} %\n" +
                                   $"EF1:\t{ef1} %\n" +
                                   $"EF2:\t{ef2} %\n" +
                                   $"EF3:\t{ef3} %\n" +
                                   $"EF4:\t{ef4} %\n" +
                                   $"EF5:\t{ef5} %\n";
                
        }

        private int FindClosestIndex(double[] arr, double val)
        {
            return arr.Select((value, index) => new { Value = value, Index = index }) // Project value and index
                    .OrderBy(x => Math.Abs(x.Value - val))                  // Order by the difference
                    .First()                                                        // Take the first item
                    .Index;
        }
    }
}
