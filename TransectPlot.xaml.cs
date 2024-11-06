using ScottPlot;
using ScottPlot.Plottable;
using System;
using System.Windows.Controls;
using System.Windows.Input;

namespace TreefallPatternAnalysis
{
    public partial class TransectPlot : UserControl
    {
        public TransectPlot()
        {
            InitializeComponent();
            overviewPlot.Plot.Style(figureBackground: System.Drawing.Color.FromArgb(255, 229, 229, 229));
            vectorsPlot.Plot.Style(figureBackground: System.Drawing.Color.FromArgb(255, 229, 229, 229));

            overviewPlot.Plot.Layout(left: 0, right: 0, bottom: 0, top: 0);
            vectorsPlot.Plot.Layout(left: 0, right: 0, bottom: 0, top: 0);

            overviewPlot.Plot.AxisScaleLock(true, EqualScaleMode.PreserveLargest);
            vectorsPlot.Plot.AxisScaleLock(true, EqualScaleMode.PreserveX);

            overviewPlot.Plot.Title("Overview");
            vectorsPlot.Plot.Title("Transect Vectors");

            vectorsPlot.Plot.AddHorizontalLine(0, color: System.Drawing.Color.Red, width: 1, style: LineStyle.Dash);

            vf = vectorsPlot.Plot.AddVectorFieldList();
            vf.Color = System.Drawing.Color.Black;
            vf.ArrowStyle.ScaledArrowheads = true;

            RefreshPlots();
        }

        private VectorFieldList vf;

        private void RescalePlot(object sender, MouseButtonEventArgs e)
        {
            if (sender is not WpfPlot || e != null && e.MiddleButton != MouseButtonState.Pressed) return;

            ((WpfPlot)sender).Plot.AxisAuto();
        }

        public void RefreshPlots()
        {
            overviewPlot.Refresh();
            vectorsPlot.Refresh();
        }

        public Plot OverviewPlot()
        {
            return overviewPlot.Plot;
        }

        public void DrawOverviewPlot(VecHashGrid vecHashGrid, ConvergenceLine convergenceLine)
        {
            Plot plt = overviewPlot.Plot;
            plt.Clear();

            var field = plt.AddVectorFieldList();
            field.Color = System.Drawing.Color.Black;
            field.ArrowStyle.ScaledArrowheads = true;

            Span<double> vecs = vecHashGrid.vectors();

            for(int i = 0; i < vecs.Length; i+=4)
            {
                field.RootedVectors.Add((new Coordinate(0.5 * (vecs[i] + vecs[i + 2]), 0.5 * (vecs[i + 1] + vecs[i + 3])), new CoordinateVector(vecs[i + 2] - vecs[i], vecs[i + 3] - vecs[i + 1])));
            }

            plt.AddScatterLines(convergenceLine.getXs(), convergenceLine.getYs(), lineWidth: 3);

            overviewPlot.Refresh();
        }

        public void UpdateVectorsPlot(double[] vecs)
        {
            vf.RootedVectors.Clear();

            for (int i = 0; i < vecs.Length; i += 4)
            {
                vf.RootedVectors.Add((new Coordinate(0.5 * (vecs[i] + vecs[i + 2]), 0.5 * (vecs[i + 1] + vecs[i + 3])), new CoordinateVector(vecs[i + 2] - vecs[i], vecs[i + 3] - vecs[i + 1])));
            }

            vectorsPlot.Plot.AxisAuto();
        }

    }
}
