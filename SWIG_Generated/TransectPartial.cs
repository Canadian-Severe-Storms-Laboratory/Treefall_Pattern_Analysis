using ScottPlot;
using System;
using System.ComponentModel.DataAnnotations;
using System.Text.Json.Serialization;

public partial class Transect
{
    public bool intialized = false;
    public ScottPlot.Plottable.Polygon box;
    private ScottPlot.Plottable.MarkerPlot marker;
    private ScottPlot.Plottable.ScatterPlot perpLine;
    private Plot plt;

    [JsonInclude]
    public double[] vmaxResults;
    [JsonInclude]
    public double[] swirlResults;
    [JsonInclude]
    public double[] rmaxResults;
    [JsonInclude]
    public double bestMatchError;

    public void Update()
    {
        marker.X = x + Math.Cos(theta) * heightOffset;
        marker.Y = y + Math.Sin(theta) * heightOffset;

        (double[], double[]) pts = PerpLine();

        perpLine.Update(pts.Item1, pts.Item2);

        pts = OutlineBox();

        box.Xs = pts.Item1;
        box.Ys = pts.Item2;
    }

    public void Render(Plot plot)
    {
        plt = plot;
        var (pX, pY) = PerpLine();

        perpLine = plt.AddScatterLines(pX, pY, color: System.Drawing.Color.DarkMagenta, lineWidth: 2);

        (pX, pY) = OutlineBox();

        box = plt.AddPolygon(pX, pY, fillColor: System.Drawing.Color.FromArgb(40, 255, 0, 0), lineWidth: 3, lineColor: System.Drawing.Color.Red);

        marker = plt.AddMarker(x + Math.Cos(theta) * heightOffset, y + Math.Sin(theta) * heightOffset, shape: MarkerShape.filledCircle, color: System.Drawing.Color.Blue, size: 7);

        intialized = true;
    }

    public void Erase()
    {
        if (!intialized) return;

        plt.Remove(box);
        plt.Remove(marker);
        plt.Remove(perpLine);
    }

    public void ChangeColor(System.Drawing.Color color)
    {
        if (intialized) box.LineColor = color;
    }

    public (double[], double[]) OutlineBox()
    {
        double[] pv = [ Math.Cos(theta), Math.Sin(theta) ];
        double[] tv = [ pv[1], -pv[0] ];

        double[] xs = new double[4];
        double[] ys = new double[4];

        xs[0] = x + pv[0] * (heightOffset + lengthAbove) + tv[0] * width;
        xs[1] = x + pv[0] * (heightOffset + lengthAbove) - tv[0] * width;
        xs[2] = x + pv[0] * (heightOffset - lengthBelow) - tv[0] * width;
        xs[3] = x + pv[0] * (heightOffset - lengthBelow) + tv[0] * width;

        ys[0] = y + pv[1] * (heightOffset + lengthAbove) + tv[1] * width;
        ys[1] = y + pv[1] * (heightOffset + lengthAbove) - tv[1] * width;
        ys[2] = y + pv[1] * (heightOffset - lengthBelow) - tv[1] * width;
        ys[3] = y + pv[1] * (heightOffset - lengthBelow) + tv[1] * width;

        return (xs, ys);
    }

    public (double[], double[]) PerpLine()
    {
        double[] pv = [ Math.Cos(theta), Math.Sin(theta) ];

        double[] xs = new double[2];
        double[] ys = new double[2];

        xs[0] = x + pv[0] * (heightOffset + lengthAbove);

        xs[1] = x + pv[0] * (heightOffset - lengthBelow);

        ys[0] = y + pv[1] * (heightOffset + lengthAbove);
        ys[1] = y + pv[1] * (heightOffset - lengthBelow);

        return (xs, ys);
    }

    public double Length()
    {
        return (Math.Floor(lengthAbove / spacing) + Math.Floor(lengthBelow / spacing)) * spacing;
    }

    public override string ToString()
    {
        return "Transect: " + name;
    }

    public static implicit operator string(Transect transect)
    {
        return transect.ToString();
    }
}

