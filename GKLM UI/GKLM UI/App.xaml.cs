using Microsoft.UI;
using Microsoft.UI.Windowing;
using Microsoft.UI.Xaml;
using System;
using System.Runtime.InteropServices;
using Windows.Graphics;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace GKLM_UI {
	/// <summary>
	/// Provides application-specific behavior to supplement the default Application class.
	/// </summary>
	public partial class App : Application {

		private IntPtr hWnd = IntPtr.Zero;
		private AppWindow appW = null;
		private OverlappedPresenter presenter = null;


		[DllImport("C:\\dev\\GKLM\\x64\\Debug\\GKLM Win32 API.dll", EntryPoint = "HookKeyboard", CharSet = CharSet.Unicode)]
		public static extern void HookKeyboard();
		/// <summary>
		/// Initializes the singleton application object.  This is the first line of authored code
		/// executed, and as such is the logical equivalent of main() or WinMain().
		/// </summary>
		public App() {
			this.InitializeComponent();
		}

		/// <summary>
		/// Invoked when the application is launched normally by the end user.  Other entry points
		/// will be used such as when the application is launched to open a specific file.
		/// </summary>
		/// <param name="args">Details about the launch request and process.</param>
		protected override void OnLaunched(Microsoft.UI.Xaml.LaunchActivatedEventArgs args) {
			m_window = new MainWindow();

			IntPtr hwnd = WinRT.Interop.WindowNative.GetWindowHandle(m_window);
			m_window.Title = "Folder Inspector (.NET 6 WinUI 3)";

			// The Window object doesn't have Width and Height properties in WInUI 3.
			// You can use the Win32 API SetWindowPos to set the Width and Height.
			WindowId wndId = Microsoft.UI.Win32Interop.GetWindowIdFromWindow(hwnd);
			appW = AppWindow.GetFromWindowId(wndId);
			presenter = appW.Presenter as OverlappedPresenter;
			presenter.IsResizable = false;

			SetWindowSize(800, 600);
			m_window.Activate();
			//HookKeyboard();
		}

		private void SetWindowSize(int width, int height) {
			appW.Resize(new SizeInt32(width, height));
		}

		private Window m_window;
	}
}
