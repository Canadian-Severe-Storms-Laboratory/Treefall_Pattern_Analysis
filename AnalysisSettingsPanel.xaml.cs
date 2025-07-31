using System;
using System.Linq;
using System.Windows;
using System.Windows.Controls;

namespace TreefallPatternAnalysis
{

    public partial class AnalysisSettingsPanel : UserControl
    {
        public Action<object, RoutedEventArgs> OnChanged
        {
            get { return (Action<object, RoutedEventArgs>)GetValue(OnChangedCallbackProperty); }
            set { SetValue(OnChangedCallbackProperty, value); }
        }

        public static readonly DependencyProperty OnChangedCallbackProperty =
            DependencyProperty.Register(
                "OnChanged",
                typeof(Action<object, RoutedEventArgs>),
                typeof(AnalysisSettingsPanel)
            );

        public AnalysisSettingsPanel()
        {
            InitializeComponent();
        }

        private void ModelTypeListView_Loaded(object sender, RoutedEventArgs e)
        {
            if (loaded) return;
            modelTypeListView.SelectAll();
        }

        public TransectAnalysisSettings GetSettings()
        {
            return new TransectAnalysisSettings()
            {
                selectedModels = [.. modelTypeListView.SelectedItems.Cast<object>().Select(item => (double)modelTypeListView.Items.IndexOf(item))],
                vtRange = (vtmin.GetNumber(), vtmax.GetNumber()),
                vrRange = (vrmin.GetNumber(), vrmax.GetNumber()),
                vsRange = (vsmin.GetNumber(), vsmax.GetNumber()),
                vcRange = (vcmin.GetNumber(), vcmax.GetNumber()),
                numOfSimulations = (int)numberOfSimulations.GetNumber(),
                threshold = cutoffThreshold.GetNumber(),
                useGustVel = (bool)gustVelCheckBox.IsChecked,
                randomizeTransect = (bool)randomizeTransectsCheckBox.IsChecked,
                patternType = patternTypeComboBox.SelectedIndex
            };
        }

        bool locked = false;

        public void SetSettings(TransectAnalysisSettings settings)
        {
            if (settings == null) return;

            locked = true;

            modelTypeListView.UnselectAll();

            if (settings.selectedModels != null)
            {
                foreach (var idx in settings.selectedModels)
                {
                    int i = (int)idx;
                    if (i >= 0 && i < modelTypeListView.Items.Count)
                        modelTypeListView.SelectedItems.Add(modelTypeListView.Items[i]);
                }
            }

            vtmin.SetNumber(settings.vtRange.Item1);
            vtmax.SetNumber(settings.vtRange.Item2);

            vrmin.SetNumber(settings.vrRange.Item1);
            vrmax.SetNumber(settings.vrRange.Item2);

            vsmin.SetNumber(settings.vsRange.Item1);
            vsmax.SetNumber(settings.vsRange.Item2);

            vcmin.SetNumber(settings.vcRange.Item1);
            vcmax.SetNumber(settings.vcRange.Item2);

            numberOfSimulations.SetNumber(settings.numOfSimulations);
            cutoffThreshold.SetNumber(settings.threshold);
            gustVelCheckBox.IsChecked = settings.useGustVel;
            randomizeTransectsCheckBox.IsChecked = settings.randomizeTransect;
            patternTypeComboBox.SelectedIndex = settings.patternType;

            locked = false;
        }

        private void SettingsChanged(object sender, RoutedEventArgs e)
        {
            if (locked || !loaded) return;

            OnChanged?.Invoke(sender, e);
        }

        private bool loaded = false;

        private void UC_Loaded(object sender, RoutedEventArgs e)
        {
            loaded = true;
        }
    }
}
