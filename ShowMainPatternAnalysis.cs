using ArcGIS.Desktop.Framework;
using ArcGIS.Desktop.Framework.Contracts;
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

            _mainpatternanalysis = new MainPatternAnalysis();
            _mainpatternanalysis.Owner = FrameworkApplication.Current.MainWindow;
            _mainpatternanalysis.Closed += (o, e) => { _mainpatternanalysis = null; };
            _mainpatternanalysis.Show();
            //uncomment for modal
            //_mainpatternanalysis.ShowDialog();
        }

    }
}
