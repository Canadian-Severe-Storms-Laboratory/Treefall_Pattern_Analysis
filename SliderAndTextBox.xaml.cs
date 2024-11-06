using System;
using System.Reactive.Linq;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;

namespace TreefallPatternAnalysis
{
    public partial class SliderAndTextBox : UserControl
    {
        public double Minimum
        {
            get { return (double)GetValue(MinimumProperty); }
            set { SetValue(MinimumProperty, value); }
        }

        public double Maximum
        {
            get { return (double)GetValue(MaximumProperty); }
            set { SetValue(MaximumProperty, value); }
        }

        public double InitialValue
        {
            get { return (double)GetValue(InitialValueProperty); }
            set { SetValue(InitialValueProperty, value); }
        }

        public double TickFrequency
        {
            get { return (double)GetValue(TickFrequencyProperty); }
            set { SetValue(TickFrequencyProperty, value); }
        }

        public string Units
        {
            get { return (string)GetValue(UnitsProperty); }
            set { SetValue(UnitsProperty, value); }
        }

        public string Title
        {
            get { return (string)GetValue(TitleProperty); }
            set { SetValue(TitleProperty, value); }
        }

        public Action<object, RoutedEventArgs> OnChanged
        {
            get { return (Action<object, RoutedEventArgs>)GetValue(OnChangedCallbackProperty); }
            set { SetValue(OnChangedCallbackProperty, value); }
        }

        public static readonly DependencyProperty TitleProperty =
            DependencyProperty.Register("Title", typeof(string), typeof(SliderAndTextBox));

        public static readonly DependencyProperty UnitsProperty =
            DependencyProperty.Register("Units", typeof(string), typeof(SliderAndTextBox));

        public static readonly DependencyProperty MinimumProperty =
            DependencyProperty.Register("Minimum", typeof(double), typeof(SliderAndTextBox));

        public static readonly DependencyProperty MaximumProperty =
            DependencyProperty.Register("Maximum", typeof(double), typeof(SliderAndTextBox));

        public static readonly DependencyProperty InitialValueProperty =
            DependencyProperty.Register("InitialValue", typeof(double), typeof(SliderAndTextBox));

        public static readonly DependencyProperty TickFrequencyProperty =
            DependencyProperty.Register("TickFrequency", typeof(double), typeof(SliderAndTextBox));

        public static readonly DependencyProperty OnChangedCallbackProperty =
            DependencyProperty.Register(
                "OnChanged",
                typeof(Action<object, RoutedEventArgs>),
                typeof(NumberBox)
            );

        public SliderAndTextBox()
        {
            InitializeComponent();
        }

        public double GetValue()
        {
            return slider.Value;
        }

        public void SetValue(double value)
        {
            slider.Value = Math.Clamp(value, slider.Minimum, slider.Maximum);
        }

        private SemaphoreSlim semA = new SemaphoreSlim(2, 2);
        private SemaphoreSlim semB = new SemaphoreSlim(2, 2);
        private SemaphoreSlim semRun = new SemaphoreSlim(1, 1);

        private async void Slider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (textBox == null || OnChanged == null) return;

            if (semB.CurrentCount != 2 || !semA.Wait(0)) return;

            await semRun.WaitAsync();

            textBox.TextChanged -= TextBox_TextChanged;
            textBox.Text = slider.Value.ToString("F1");
            textBox.TextChanged += TextBox_TextChanged;

            OnChanged(this, e);

            await Task.Delay(10);

            semA.Release();
            semRun.Release();
        }

        private async void TextBox_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (slider == null || OnChanged == null) return;

            if (semB.CurrentCount != 2 || !semA.Wait(0)) return;

            await semRun.WaitAsync();

            if (double.TryParse(textBox.Text, out double newValue))
            {
                slider.ValueChanged -= Slider_ValueChanged;
                slider.Value = Math.Clamp(newValue, slider.Minimum, slider.Maximum);
                slider.ValueChanged += Slider_ValueChanged;

                OnChanged(this, e);
                
            }

            await Task.Delay(10);

            semA.Release();
            semRun.Release();
        }

        private void UserControl_Loaded(object sender, RoutedEventArgs e)
        {
            slider.Minimum = Minimum;
            slider.Maximum = Maximum;
            slider.Value = InitialValue;
            slider.TickFrequency = TickFrequency;
            textBox.Text = slider.Value.ToString("F1");
        }
    }
}
