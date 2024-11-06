using System;
using System.Windows.Threading;
using TreefallPatternAnalysis;

public partial class Monitor
{
    private ProgressWindow progressWindow;
    private DispatcherTimer timer;

    public void Start()
    {
        progressWindow = new ProgressWindow();
        progressWindow.Show();
        progressWindow.Focus();

        progressWindow.Closed += Cancel;

        timer = new DispatcherTimer
        {
            Interval = TimeSpan.FromMilliseconds(100)
        };

        timer.Tick += Update;
        timer.Start();
    }

    public void Stop() { 
        timer.Stop();
        progressWindow.Closed -= Cancel;
        progressWindow.Close();
    }

    private void Update(object sender, EventArgs e)
    {
        progressWindow.Title = this.title;
        progressWindow.progressBar.Value = this.value;
        progressWindow.progressBar.Minimum = this.min;
        progressWindow.progressBar.Maximum = this.max;
        progressWindow.countLabel.Content = this.value.ToString("N0") + " / " + this.max.ToString("N0");
        progressWindow.message.Content = this.message;
        progressWindow.warning.Content = this.warning;
    }

    private void Cancel(object sender, EventArgs e)
    {
        cancelled = true;
    }
}
