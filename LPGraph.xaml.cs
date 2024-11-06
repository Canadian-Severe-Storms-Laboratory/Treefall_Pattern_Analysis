﻿using ScottPlot;
using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Controls;

namespace TreefallPatternAnalysis
{
    public partial class LPGraph : UserControl
    {
        public static readonly RoutedEvent UpdateDataEvent = EventManager.RegisterRoutedEvent("UpdateData_LPGraph", RoutingStrategy.Bubble, typeof(RoutedEventHandler), typeof(CustomModelParameters));

        public event RoutedEventHandler UpdateData
        {
            add { AddHandler(UpdateDataEvent, value); }
            remove { RemoveHandler(UpdateDataEvent, value); }
        }

        public System.Drawing.Color color = System.Drawing.Color.Red;
        private double[] x;
        private double[] y;
        private ScottPlot.Plottable.ScatterPlotDraggable spd;
        private int rmaxIdx;


        public LPGraph()
        {
            InitializeComponent();

            plot.Plot.Layout(left: 0, right: 0, bottom: 0, top: 0, padding: 0);

            //x = [0.0, 0.2, 0.55, 1.0, 2.0, 4.0, 10.0];
            //y = [0.0, 0.5, 0.89, 1.0, 0.7, 0.43, 0.0];

            x = [0.0, 1.0, 1.1, 1.25, 1.5, 2.0, 3.0, 4.0, 5.0, 7.0, 10.0];
            y = [0.0, 1.0, 1.0/1.1, 1.0/1.25, 1.0/1.5, 1.0/2.0, 1.0/3.0, 1.0/4.0, 1.0/5.0, 1.0/7.0, 0.0];

            rmaxIdx = Array.IndexOf(x, 1.0);

            UpdateSpline();
            plot.Refresh();
        }

        public double[] GetLines()
        {
            List<double> lineData = [];

            for (int i = 1; i < x.Length; i++)
            {
                double maxR = x[i];
                double m = (y[i] - y[i - 1]) / (x[i] - x[i - 1]);
                double b = y[i] - m * x[i];

                lineData.Add(maxR);
                lineData.Add(m);
                lineData.Add(b);
            }

            return lineData.ToArray();
        }

        public void UpdateSpline()
        {
            var plt = plot.Plot;

            if (spd != null) plt.Remove(spd);

            spd = new ScottPlot.Plottable.ScatterPlotDraggable(x, y)
            {
                DragEnabled = true,
                DragXLimitMin = 0,
                DragXLimitMax = 10,
                DragYLimitMin = 0,
                DragYLimitMax = 1,
                MarkerSize = 10,
                MarkerShape = MarkerShape.openSquare,
                MarkerColor = color,
                LineWidth = 2,
            };

            plt.Add(spd);

        }

        public char lastKey = '\0';
        private int selectedIdx = -1;

        private void NodeMoved(object sender, RoutedEventArgs e)
        {
            var plottable = e.OriginalSource as ScottPlot.Plottable.ScatterPlotDraggable;

            if (selectedIdx == -1)
            {
                selectedIdx = plottable.CurrentIndex;
            }
            else if (selectedIdx != plottable.CurrentIndex) return;

            if (selectedIdx == 0)
            {
                x[selectedIdx] = 0.0;
                y[selectedIdx] = 0.0;

                return;
            }

            if (lastKey == 'n')
            {
                AddNode(selectedIdx);
            }

            if (selectedIdx == x.Length - 1)
            {
                x[selectedIdx] = 10.0;
                y[selectedIdx] = 0.0;

                return;
            }
            else if (selectedIdx == rmaxIdx)
            {
                x[selectedIdx] = 1.0;
                y[selectedIdx] = 1.0;

                return;
            }

            if (lastKey == 'd')
            {
                DeleteNode(selectedIdx);
                return;
            }

            x[selectedIdx] = Math.Clamp(x[selectedIdx], x[selectedIdx - 1] + 0.01, x[selectedIdx + 1] - 0.01);

            if (selectedIdx < rmaxIdx)
            {
                y[selectedIdx] = Math.Clamp(y[selectedIdx], y[selectedIdx - 1] + 0.01, y[selectedIdx + 1] - 0.01);
            }
            else
            {
                y[selectedIdx] = Math.Clamp(y[selectedIdx], y[selectedIdx + 1] + 0.01, y[selectedIdx - 1] - 0.01);
            }

        }

        private void NodeDropped(object sender, RoutedEventArgs e)
        {
            selectedIdx = -1;
            RaiseEvent(new RoutedEventArgs(UpdateDataEvent));
        }

        private void DeleteNode(int idx)
        {
            lastKey = '\0';

            selectedIdx = -1;
            if (idx < rmaxIdx) rmaxIdx--;

            x = x.RemoveAt(idx);
            y = y.RemoveAt(idx);

            UpdateSpline();
            RaiseEvent(new RoutedEventArgs(UpdateDataEvent));
        }

        private void AddNode(int idx)
        {
            lastKey = '\0';
            selectedIdx++;

            if (idx <= rmaxIdx) rmaxIdx++;

            double mx = (x[idx] + x[idx - 1]) / 2.0;
            double my = (y[idx] + y[idx - 1]) / 2.0;

            x = x.InsertAt(idx, mx);
            y = y.InsertAt(idx, my);

            UpdateSpline();
            RaiseEvent(new RoutedEventArgs(UpdateDataEvent));
        }

    }

    public static class ArrayExtensions
    {
        public static T[] RemoveAt<T>(this T[] source, int index)
        {
            T[] dest = new T[source.Length - 1];
            if (index > 0)
                Array.Copy(source, 0, dest, 0, index);

            if (index < source.Length - 1)
                Array.Copy(source, index + 1, dest, index, source.Length - index - 1);

            return dest;
        }

        public static T[] InsertAt<T>(this T[] source, int index, T element)
        {
            T[] dest = new T[source.Length + 1];

            // Copy elements before the index
            if (index > 0)
                Array.Copy(source, 0, dest, 0, index);

            // Insert the new element
            dest[index] = element;

            // Copy elements after the index
            if (index < source.Length)
                Array.Copy(source, index, dest, index + 1, source.Length - index);

            return dest;
        }
    }
}
