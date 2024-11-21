using ArcGIS.Desktop.Framework;
using ArcGIS.Desktop.Framework.Contracts;
using static ArcGISUtils.Utils;

namespace TreefallPatternAnalysis
{
    internal class ShowCustomModelProWindow : Button
{

    private CustomModelProWindow _custommodelprowindow = null;

    protected override void OnClick()
    {
        //already open?
        if (_custommodelprowindow != null)
            return;

        PreLoadDlls();

        _custommodelprowindow = new CustomModelProWindow();
        _custommodelprowindow.Owner = FrameworkApplication.Current.MainWindow;
        _custommodelprowindow.Closed += (o, e) => { _custommodelprowindow = null; };
        _custommodelprowindow.Show();
         //uncomment for modal
         //_custommodelprowindow.ShowDialog();
}

}
}
