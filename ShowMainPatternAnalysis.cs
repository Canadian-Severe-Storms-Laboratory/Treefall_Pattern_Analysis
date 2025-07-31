using ArcGIS.Desktop.Framework;
using ArcGIS.Desktop.Framework.Contracts;
using System;
using System.Windows;
using static ArcGISUtils.Utils;

namespace TreefallPatternAnalysis
{
    internal class ShowMainPatternAnalysis : Button
    {

        private MainPatternAnalysis _mainpatternanalysis = null;

        protected override void OnClick()
        {
            //already open?
            if (_mainpatternanalysis != null)
                return;

            PreLoadDlls();

            try
            {
                _mainpatternanalysis = new MainPatternAnalysis();
                _mainpatternanalysis.Owner = FrameworkApplication.Current.MainWindow;
                _mainpatternanalysis.Closed += (o, e) => { _mainpatternanalysis = null; };
                _mainpatternanalysis.Show();
            }
            catch (Exception e)
            {
                MessageBox.Show("An unhandled Error has Occurred" + e.Message);
                return;
            }

            //uncomment for modal
            //_mainpatternanalysis.ShowDialog();
        }

    }
}
