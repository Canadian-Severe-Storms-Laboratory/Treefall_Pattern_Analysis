using ScottPlot;
using ScottPlot.Drawing;
using ScottPlot.Plottable;
using ScottPlot.Statistics;
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Windows;
using System.Windows.Documents;
using System.Windows.Input;

namespace TreefallPatternAnalysis
{
    public partial class CustomModelProWindow : ArcGIS.Desktop.Framework.Controls.ProWindow
    {
        private Plot plt;
        private VortexModel model;
        private double[,] magnitudes;
        private Vector2[,] unitVecs;
        private double[] xPositions;
        private double[] yPositions;
        private double dx;

        public CustomModelProWindow()
        {
            InitializeComponent();

            customModelParameters.UpdateData += UpdateGraph;
            vrLpGraph.UpdateData += UpdateGraph;
            vtLpGraph.UpdateData += UpdateGraph;
            vtLpGraph.color = System.Drawing.Color.Green;
            vtLpGraph.UpdateGraph();

            plt = graphPlot.Plot;
            PixelPadding padding = new PixelPadding(120f, 170f, 20f, 10f);
            plt.ManualDataArea(padding);

            graphPlot.Configuration.AllowDroppedFramesWhileDragging = true;
            graphPlot.Configuration.Quality = ScottPlot.Control.QualityMode.LowWhileDragging;
        }

        private void ProWindow_Loaded(object sender, RoutedEventArgs e)
        {
            plt.SetAxisLimits(-400.0, 500.0, -600.0, 300.0);
            UpdateGraph(null, null);
        }

        private bool debounce = false;

        private void KeyPressed(object sender, KeyEventArgs e)
        {
            if (debounce) return;

            vrLpGraph.lastKey = e.Key.ToString().ToLower()[0];
            vtLpGraph.lastKey = e.Key.ToString().ToLower()[0];
            debounce = true;
        }

        private void KeyReleased(object sender, KeyEventArgs e)
        {
            vrLpGraph.lastKey = '\0';
            vtLpGraph.lastKey = '\0';
            debounce = false;
        }

        private void UpdateGraph(object sender, RoutedEventArgs e)
        {
            if (graphPlot == null) return;

            RenderFieldGraph();

            graphPlot.Refresh();
           
        }

        private void MouseWheelChanged(object sender, MouseWheelEventArgs e)
        {
            UpdateGraph(null, null);
        }

        private void MouseMoved(object sender, MouseEventArgs e)
        {
            if (e.LeftButton == MouseButtonState.Pressed)
            {
                UpdateGraph(null, null);
            }
        }

        private void RenderFieldGraph()
        {
            vrLpGraph.displayLPModel = true;
            vtLpGraph.displayLPModel = true;
            var vrLines = vrLpGraph.GetLines();
            var vtLines = vtLpGraph.GetLines();

            var modelParams = customModelParameters.GetParams();
            int n = modelParams.n;
            double rmax = modelParams.rmax;

            model = GetModel(modelParams, vrLines, vtLines);

            if (modelParams.modelType < 6)
            {
                SetCustomGraph(modelParams);
            }

            vrLpGraph.UpdateGraph();
            vtLpGraph.UpdateGraph();
            vrLpGraph.ResetAxes();
            vtLpGraph.ResetAxes();

            if (model.hasPattern())
            {
                model.solveAxesOfInterest();
            }

            var limits = RenderHeatMap(n, rmax);

            if (modelParams.displayRmax)
            {
                plt.AddCircle(0, 0, rmax, System.Drawing.Color.Black, 1, lineStyle: LineStyle.DashDot);
            }

            if (modelParams.displayCurve)
            {
                //var curve = model.maxVelCurve(-5.0, 5.0, 500);
                var curve = model.patternCurve(5000);

                double[] xs = curve.xs;
                double[] ys = curve.ys;

                if (xs.Length > 0)
                {
                    for (int i = 0; i < xs.Length; i++)
                    {
                        xs[i] *= rmax;
                        ys[i] *= rmax;
                    }

                    plt.AddScatter(xs, ys, System.Drawing.Color.White, 4, 1);
                }
            }

            if (modelParams.displayVectors)
            {
                var vf = plt.AddVectorField(unitVecs, xPositions, yPositions, null, System.Drawing.Color.Black, null, dx);
                vf.ScaledArrowheads = true;
                vf.ScaledArrowheadLength = 0.4;
            }

            var pattern = model.pattern(16);

            Span<double> p = pattern.vecs;

            var vfp = plt.AddVectorFieldList();
            vfp.Color = System.Drawing.Color.Black;
            vfp.ArrowStyle.LineWidth = 2;
            vfp.ArrowStyle.ScaledArrowheads = true;
            vfp.ArrowStyle.ScaledArrowheadLength = 0.4;

            for (int i = 0; i < p.Length; i += 4)
            {
                vfp.RootedVectors.Add((new Coordinate(p[i] * rmax, p[i + 1] * rmax), new CoordinateVector(p[i + 2] * 0.2 * rmax, p[i + 3] * 0.2 * rmax)));
            }


            patternPlot.Display(pattern);

            plt.SetAxisLimits(limits);
        }

        private AxisLimits RenderHeatMap(int n, double rmax)
        {
            var limits = plt.GetAxisLimits();
            double xMin = limits.XMin;
            double xMax = limits.XMax;
            double yMin = limits.YMin;
            double yMax = limits.YMax;

            plt.Clear();

            Span<double> field = model.field(xMin / rmax, xMax / rmax, yMin / rmax, yMax / rmax, n);

            magnitudes = new double[n, n];
            unitVecs = new Vector2[n, n];
            xPositions = new double[n];
            yPositions = new double[n];

            dx = (xMax - xMin) / n;

            for (int i = 0; i < n; i++)
            {
                //0.5 to center vectors to grid square
                yPositions[i] = yMax - (i + 0.5) * dx;

                for (int j = 0; j < n; j++)
                {
                    int idx = 2 * (i * n + j);

                    double vx = field[idx];
                    double vy = field[idx + 1];

                    magnitudes[i, j] = Math.Sqrt(vx * vx + vy * vy);

                    if (magnitudes[i, j] < 1e-7)
                    {
                        unitVecs[j, i] = new Vector2(0.0f, 0.0f);
                    }
                    else
                    {
                        unitVecs[j, i] = new Vector2(vx / magnitudes[i, j], vy / magnitudes[i, j]);
                    }

                    if (i != 0) continue;

                    xPositions[j] = xMin + (j + 0.5) * dx;
                }
            }

            Heatmap hm = plt.AddHeatmap(magnitudes, Colormap.Jet);
            hm.OffsetX = xMin;
            hm.OffsetY = yMin;
            hm.CellHeight = dx;
            hm.CellWidth = dx;
            hm.Smooth = true;
            hm.UseParallel = true;
            hm.Update(magnitudes, Colormap.Jet, 0.0, 2.75);

            Colorbar colorbar = plt.AddColorbar(hm);
            colorbar.MinValue = 0.0;
            colorbar.MaxValue = 2.75;
            colorbar.Label = "Wind Velocity (ratio to Vc)";

            return limits;
        }

        private static VortexModel GetModel(CustomModelParameters.Params modelParams, double[] vrLines, double[] vtLines)
        {
            return modelParams.modelType switch
            {
                0 => new ModifiedRankineVortex(modelParams.phi, modelParams.vr, modelParams.vt, modelParams.vs),
                1 => new BakerSterlingVortex(modelParams.vr, modelParams.vt, modelParams.vs),
                2 => new BurgersRottVortex(modelParams.vr, modelParams.vt, modelParams.vs),
                3 => new SullivanVortex(modelParams.vr, modelParams.vt, modelParams.vs),
                4 => new BurgersRottRRVortex(modelParams.vr, modelParams.vt, modelParams.vs),
                5 => new SullivanRRVortex(modelParams.vr, modelParams.vt, modelParams.vs),
                _ => new LinearPiecewiseVortex(vrLines, vtLines)
                {
                    Vr = modelParams.vr,
                    Vt = modelParams.vt,
                    Vs = modelParams.vs,
                },
            };
        }

        private void SetCustomGraph(CustomModelParameters.Params modelParams)
        {
            vrLpGraph.displayLPModel = false;
            vtLpGraph.displayLPModel = false;

            switch (modelParams.modelType) {
                case 1:
                    vrLpGraph.customModelFunction = (x) => x < 0.0 ? null : 2.0 * x / (x * x + 1.0);
                    vtLpGraph.customModelFunction = (x) => x < 0.0 ? null : 2.0 * x / (x * x + 1.0);
                    break;

                case 2:
                    vrLpGraph.customModelFunction = (x) => x < 0.0 ? null : (311150184667e-11 * x + 1e0 * x * x * x) / (182417933e-8 + 151889411e-8 * x * x + 768428406667e-12 * x * x * x * x);
                    vtLpGraph.customModelFunction = (x) => x < 0.0 ? null : (311150184667e-11 * x + 1e0 * x * x * x) / (182417933e-8 + 151889411e-8 * x * x + 768428406667e-12 * x * x * x * x);
                    break;

                case 3:
                    vrLpGraph.customModelFunction = (x) => x < 0.0 ? null : (607023333333e-13 * x + 2290069e-6 * x * x * x) / (1118044e-6 + -1e0 * x * x + 223272733333e-11 * x * x * x * x);
                    vtLpGraph.customModelFunction = (x) => x < 0.0 ? null : (607023333333e-13 * x + 2290069e-6 * x * x * x) / (1118044e-6 + -1e0 * x * x + 223272733333e-11 * x * x * x * x);
                    break;

                case 4:
                    vrLpGraph.customModelFunction = (x) => x < 0.0 ? null : (x < 1.0 ? x : 1.0 / x);
                    vtLpGraph.customModelFunction = (x) => x < 0.0 ? null : (311150184667e-11 * x + 1e0 * x * x * x) / (182417933e-8 + 151889411e-8 * x * x + 768428406667e-12 * x * x * x * x);
                    break;

                case 5:
                    vrLpGraph.customModelFunction = (x) => x < 0.0 ? null : (x < 1.0 ? x : 1.0 / x);
                    vtLpGraph.customModelFunction = (x) => x < 0.0 ? null : (607023333333e-13 * x + 2290069e-6 * x * x * x) / (1118044e-6 + -1e0 * x * x + 223272733333e-11 * x * x * x * x);
                    break;

                default:
                    vrLpGraph.customModelFunction = (x) => x < 0.0 ? null : (x < 1.0 ? Math.Pow(x, modelParams.phi) : 1.0 / Math.Pow(x, modelParams.phi));
                    vtLpGraph.customModelFunction = (x) => x < 0.0 ? null : (x < 1.0 ? Math.Pow(x, modelParams.phi) : 1.0 / Math.Pow(x, modelParams.phi));
                    break;
            } 
        }
    }
}
