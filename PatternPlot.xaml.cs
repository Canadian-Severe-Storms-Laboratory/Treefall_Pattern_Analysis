using ScottPlot;
using ScottPlot.Plottable;
using System;
using System.Windows.Controls;
using System.Windows.Input;

namespace TreefallPatternAnalysis
{
    public partial class PatternPlot : UserControl
    {
        private VectorFieldList obsVF;
        private VectorFieldList simVF;
        public double simScale = 1.0;

        public PatternPlot()
        {
            InitializeComponent();

            var plt = patternPlot.Plot;

            plt.Style(figureBackground: System.Drawing.Color.FromArgb(255, 229, 229, 229));
            plt.Layout(0, 0, 0, 0, 0);
            plt.AxisScaleLock(true, EqualScaleMode.PreserveY);

            plt.Title("Pattern");

            plt.AddHorizontalLine(0, color: System.Drawing.Color.Red, width: 1, style: LineStyle.Dash);

            obsVF = plt.AddVectorFieldList();
            obsVF.Color = System.Drawing.Color.Black;
            obsVF.ArrowStyle.LineWidth = 2;
            obsVF.ArrowStyle.ScaledArrowheads = true;
            obsVF.ArrowStyle.ScaledArrowheadLength = 0.4;

            simVF = plt.AddVectorFieldList();
            simVF.Color = System.Drawing.Color.DarkRed;
            simVF.ArrowStyle.LineWidth = 2;
            simVF.ArrowStyle.ScaledArrowheads = true;
            simVF.ArrowStyle.ScaledArrowheadLength = 0.4;

            patternPlot.Refresh();
        }

        public void Display(Pattern pattern)
        {
            double dx = pattern.dx * simScale;
            double Xc = pattern.Xc;

            simVF.RootedVectors.Clear();
            
            Span<double> p = pattern.vecs;

            double min = p[0];
            double max = p[0];

            for (int i = 0; i < p.Length; i+=4)
            {
                //rotate pattern
                double y = (Xc - p[i]) * simScale;
                double vx = p[i + 3];
                double vy = -p[i + 2];

                min = Math.Min(min, y);
                max = Math.Max(max, y);

                simVF.RootedVectors.Add((new Coordinate(0.0, y), new CoordinateVector(vx * dx, vy * dx)));
            }

            patternPlot.Plot.SetAxisLimitsX(-2.0 * dx, 2.0 * dx);
            patternPlot.Plot.SetAxisLimitsY(min - 0.5 * dx, max + 0.5 * dx);
            patternPlot.Refresh();
        }

        public void Display(ObservedPattern pattern)
        {
            obsVF.RootedVectors.Clear();

            double dx = pattern.spacing;
            Span<double> p = pattern.vectors();

            for (int i = 0; i < p.Length; i += 3)
            {
                obsVF.RootedVectors.Add((new Coordinate(0.01, p[i]), new CoordinateVector(p[i + 1] * dx, p[i + 2] * dx)));
            }

            patternPlot.Plot.SetAxisLimitsX(-2.0 * dx, 2.0 * dx);
            patternPlot.Plot.SetAxisLimitsY(-pattern.lengthBelow - 0.5 * dx, pattern.lengthAbove + 0.5 * dx);
            patternPlot.Refresh();
        }

        public void RemoveSimPattern(){ simVF.RootedVectors.Clear(); patternPlot.Refresh(); }

        public void RemoveObsPattern(){ obsVF.RootedVectors.Clear(); patternPlot.Refresh(); }

        private void OverrideMiddleMouse(object sender, MouseButtonEventArgs e)
        {
            if (sender is not WpfPlot || e != null && e.MiddleButton != MouseButtonState.Pressed) return;
            e.Handled = true;
        }

        private void patternPlot_Loaded(object sender, System.Windows.RoutedEventArgs e)
        {
            
        }
    }
}
