using ArcGIS.Desktop.Internal.Mapping;
using ScottPlot;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows;
using System.Windows.Controls;

namespace TreefallPatternAnalysis
{
    public partial class TransectCreationList : UserControl
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
                typeof(TransectCreationList)
            );

        //private int numTransectsCreated = 0;

        public TransectCreationList()
        {
            InitializeComponent();
        }

        private void TransectSelected(object sender, SelectionChangedEventArgs e)
        {
            if (transectList.SelectedItem == null) return;

            for (int i = 0; i < transectList.Items.Count; i++)
            {
                (transectList.Items[i] as Transect).ChangeColor(System.Drawing.Color.Turquoise);
            }

            (transectList.SelectedItem as Transect).ChangeColor(System.Drawing.Color.Red);

            OnChanged(this, e);
        }

        private void RemoveTransect(object sender, RoutedEventArgs e)
        {
            if (transectList.SelectedItem == null) return;

            (transectList.SelectedItem as Transect).Erase();

            transectList.Items.Remove(transectList.SelectedItem);

            if (transectList.Items.IsEmpty)
            {
                transectList.SelectedIndex = -1;
                OnChanged(this, e);
            }
            else
            {
                transectList.SelectedIndex = 0;
            }         
        }

        private void NewTransect(object sender, RoutedEventArgs e)
        {
            int count = transectList.Items.Count;
            transectList.Items.Add(new Transect((count + 1).ToString()));
            transectList.SelectedIndex = transectList.Items.Count - 1;
        }

        private void SortTransects(object sender, RoutedEventArgs e)
        {
            List<Transect> transects = transectList.Items.OfType<Transect>().ToList();

            transects.Sort((a, b) => a.positionOffset.CompareTo(b.positionOffset));
            transectList.Items.Clear();

            for (int i = 0; i < transects.Count; i++)
            {
                transects[i].name = (i + 1).ToString();
                transectList.Items.Add(transects[i]);
            }
        }

        public void LoadFromSave(List<Transect> list, Plot plt)
        {
            transectList.Items.Clear();

            for(int i = 0; i < list.Count; i++)
            {
                Transect transect = list[i];
                transect.Render(plt);
                transect.ChangeColor(System.Drawing.Color.Turquoise);
                transectList.Items.Add(transect);
            }

            transectList.SelectedIndex = transectList.Items.Count - 1;
            (transectList.SelectedItem as Transect).ChangeColor(System.Drawing.Color.Red);
        }

        public void Add(Transect transect)
        {
            transect.name = (transectList.Items.Count).ToString();
            transectList.Items.Add(transect);

            for (int i = 0; i < transectList.Items.Count; i++)
            {
                (transectList.Items[i] as Transect).ChangeColor(System.Drawing.Color.Turquoise);
            }

            transectList.SelectedIndex = transectList.Items.Count - 1;

            (transectList.SelectedItem as Transect).ChangeColor(System.Drawing.Color.Red);    
        }

        public List<Transect> Transects()
        {
            var transects = new List<Transect>();

            foreach (Transect transect in transectList.Items)
            {
                transects.Add(transect);
            }

            return transects;
        }

        public Transect SelectedTransect()
        {
            return transectList.SelectedItem as Transect;
        }

        public int SelectedIndex()
        {
            return transectList.SelectedIndex;
        } 

        public int Count()
        {
            return transectList.Items.Count;
        }   

        public void Select(int index)
        {
            transectList.SelectedIndex = index;
        }
    }
}
