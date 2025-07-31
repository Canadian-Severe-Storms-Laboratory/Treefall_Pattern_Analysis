using ScottPlot;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

    public partial class Range : global::System.IDisposable
    {
        public static implicit operator Range((double, double) inVal)
        {
            return new() { min = inVal.Item1, max = inVal.Item2 };
        }
    }

