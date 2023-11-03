using Microsoft.UI;
using Microsoft.UI.Windowing;
using Microsoft.UI.Xaml;
using System;
using System.Runtime.InteropServices;
using Windows.Graphics;

namespace GKLM_UI {
	public partial class App : Application {

		private AppWindow appW = null;
		private OverlappedPresenter presenter = null;

		[DllImport("C:\\dev\\GKLM\\x64\\Debug\\GKLM Win32 API.dll", EntryPoint = "HookKeyboard", CharSet = CharSet.Unicode)]
		public static extern void HookKeyboard();

		public App() {
			this.InitializeComponent();
		}

		protected override void OnLaunched(Microsoft.UI.Xaml.LaunchActivatedEventArgs args) {
			m_window = new MainWindow();

			IntPtr hwnd = WinRT.Interop.WindowNative.GetWindowHandle(m_window);
			m_window.Title = "Folder Inspector (.NET 6 WinUI 3)";
			WindowId wndId = Microsoft.UI.Win32Interop.GetWindowIdFromWindow(hwnd);
			appW = AppWindow.GetFromWindowId(wndId);
			presenter = appW.Presenter as OverlappedPresenter;
			presenter.IsResizable = false;

			SetWindowSize(800, 600);
			m_window.Activate();
			HookKeyboard();
		}

		private void SetWindowSize(int width, int height) {
			appW.Resize(new SizeInt32(width, height));
		}

		private Window m_window;
	}
}
