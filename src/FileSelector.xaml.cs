using Microsoft.Win32;
using System;
using System.Windows;
using System.Windows.Controls;

namespace TreefallPatternAnalysis
{
    public partial class FileSelector : UserControl
    {
        public Action<object, RoutedEventArgs> OnSelection
        {
            get { return (Action<object, RoutedEventArgs>)GetValue(OnSelectionCallbackProperty); }
            set { SetValue(OnSelectionCallbackProperty, value); }
        }

        public static readonly DependencyProperty OnSelectionCallbackProperty =
            DependencyProperty.Register(
                "OnSelection",
                typeof(Action<object, RoutedEventArgs>),
                typeof(TransectCreationList)
            );

        private string filePath = null;

        public FileSelector()
        {
            InitializeComponent();
        }

        private void SelectFile(object sender, RoutedEventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();

            openFileDialog.Filter = "JSON|*.json";

            if (openFileDialog.ShowDialog() == true)
            {
                filePath = openFileDialog.FileName;
                OnSelection(null, null);
            }
        }

        public bool IsEmpty()
        {
            return filePath == null;
        }

        public string GetFilePath()
        {
            return filePath;
        }
    }
}
