using Microsoft.UI.Xaml;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace GKLM_UI {
	/// <summary>
	/// An empty window that can be used on its own or navigated to within a Frame.
	/// </summary>
	public sealed partial class MainWindow : Window {

		List<Process> processlist;
		Process attachedProgress;
		ObservableCollection<Process> windowTitles = new ObservableCollection<Process>();

		[DllImport("C:\\dev\\GKLM\\x64\\Debug\\GKLM Win32 API.dll")]
		public static extern void PassHandle(IntPtr hWnd);

		public MainWindow() {
			this.InitializeComponent();
			// Hide default title bar.
			ExtendsContentIntoTitleBar = true;
			SetTitleBar(TitleBar);
		}

		private void WindowsCombo_DropDownOpened(object sender, object e) {
			windowTitles.Clear();
			processlist = Process.GetProcesses().ToList();
			foreach (Process process in processlist) {
				if (!String.IsNullOrEmpty(process.MainWindowTitle)) {
					windowTitles.Add(process/* + " [" + process.MainWindowTitle + "]" + " [" + process.Id + "]"*/);
				}
			}
		}

		private void WindowSelect_Click(object sender, RoutedEventArgs e) {
			attachedProgress = (Process)WindowsCombo.SelectionBoxItem;
			if (attachedProgress != null) {
				PassHandle(attachedProgress.MainWindowHandle);
			}
		}
	}
}
