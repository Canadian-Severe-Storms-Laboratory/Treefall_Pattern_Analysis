using ScottPlot.Plottable;
using ScottPlot;
using System.Windows.Input;
using System.Windows;
using System;
using ScottPlot.Statistics;
using ScottPlot.Drawing;
using System.Runtime.InteropServices;

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
            vtLpGraph.UpdateSpline();

            plt = graphPlot.Plot;
            PixelPadding padding = new PixelPadding(120f, 170f, 20f, 10f);
            plt.ManualDataArea(padding);

            plt.SetAxisLimits(-10.0, 10.0, -10.0, 10.0);

            graphPlot.Configuration.AllowDroppedFramesWhileDragging = true;
            graphPlot.Configuration.Quality = ScottPlot.Control.QualityMode.LowWhileDragging;

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
            var modelParams = customModelParameters.GetParams();
            var vrLines = vrLpGraph.GetLines();
            var vtLines = vtLpGraph.GetLines();

            int n = modelParams.n;

            model = new LinearPiecewiseVortex(vrLines, vtLines)
            {
                Vr = modelParams.vr,
                Vt = modelParams.vt,
                Vs = modelParams.vs,
            };

            //model = new ModifiedRankineVortex(0.63662, modelParams.vr, modelParams.vt, modelParams.vs);

            if (model.hasPattern())
            {
                model.solveAxesOfInterest();
            }

            var limits = RenderHeatMap(n);

            if (modelParams.displayRmax)
            {
                plt.AddCircle(0, 0, 1.0, System.Drawing.Color.Black, 1, lineStyle: LineStyle.DashDot);
            }

            if (modelParams.displayCurve)
            {
                var curve = model.patternCurve(500);

                double[] xs = curve.xs;
                double[] ys = curve.ys;

                if (xs.Length > 0)
                {
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
                vfp.RootedVectors.Add((new Coordinate(p[i], p[i+1]), new CoordinateVector(p[i+2]*0.2, p[i+3]*0.2)));
            }


            patternPlot.Display(pattern);

            plt.SetAxisLimits(limits);
        }

        private AxisLimits RenderHeatMap(int n)
        {
            var limits = plt.GetAxisLimits();
            double xMin = limits.XMin;
            double xMax = limits.XMax;
            double yMin = limits.YMin;
            double yMax = limits.YMax;

            plt.Clear();

            Span<double> field = model.field(xMin, xMax, yMin, yMax, n);

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
            hm.Update(magnitudes, Colormap.Jet, 0.0, 2.75); //120.0
            hm.OffsetX = xMin;
            hm.OffsetY = yMin;
            hm.CellHeight = dx;
            hm.CellWidth = dx;
            hm.Smooth = true;
            hm.UseParallel = true;

            Colorbar colorbar = plt.AddColorbar(hm);
            colorbar.MinValue = 0.0;
            colorbar.MaxValue = 2.75; //120.0
            colorbar.Label = "Wind Velocity (ratio to Vc)";

            return limits;
        }

        private void ProWindow_Loaded(object sender, RoutedEventArgs e)
        {

        }
    }
}
