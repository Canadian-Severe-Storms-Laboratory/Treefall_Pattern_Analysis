using System;
using System.Globalization;
using System.Reflection;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

namespace TreefallPatternAnalysis
{
    /// <summary>
    /// Interaction logic for NumberBox.xaml
    /// </summary>
    public partial class NumberBox : UserControl
    {

        public string NumberString
        {
            get { return (string)GetValue(numberProperty); }
            set { SetValue(numberProperty, value); }
        }

        public static readonly DependencyProperty numberProperty = DependencyProperty.Register(
            "NumberString",
            typeof(string),
            typeof(NumberBox)
        );

        public static readonly DependencyProperty OnTextChangedCallbackProperty =
        DependencyProperty.Register(
            "TextChanged",          
            typeof(Action<object, RoutedEventArgs>),           
            typeof(NumberBox),                
            new PropertyMetadata(null));      

        public Action<object, RoutedEventArgs> TextChanged
        {
            get { return (Action<object, RoutedEventArgs>)GetValue(OnTextChangedCallbackProperty); }
            set { SetValue(OnTextChangedCallbackProperty, value); }
        }

        public NumberBox()
        {
            InitializeComponent();
        }

        public double GetNumber()
        {
            if (NumberString == null || NumberString.Length == 0)
            {
                return 0.0;
            }

            return double.Parse(NumberString, CultureInfo.InvariantCulture);
        }

        public void SetNumber(double number)
        {
            NumberString = Math.Round(number, 2).ToString(CultureInfo.InvariantCulture);
        }

        public int GetIntNumber()
        {
            if (NumberString == null || NumberString.Length == 0)
            {
                return 0;
            }

            return (int)Math.Round(double.Parse(NumberString, CultureInfo.InvariantCulture));
        }

        private void NumberValidationTextBox(object sender, TextCompositionEventArgs e)
        {

            var textBox = sender as TextBox;

            var fullText = textBox.Text.Insert(textBox.SelectionStart, e.Text);

            if (fullText == "-")
            {
                e.Handled = false;
                return;
            }

            double val;
            // If parsing is successful, set Handled to false
            e.Handled = !double.TryParse(fullText,
                                         NumberStyles.AllowDecimalPoint | NumberStyles.AllowLeadingSign,
                                         CultureInfo.InvariantCulture,
                                         out val);
        }

        private void TextBox_TextChanged(object sender, TextChangedEventArgs e)
        {
            var textBox = sender as TextBox;
            if (textBox != null && TextChanged != null)
            {
                TextChanged(this, e);
            }
        }
    }
}
