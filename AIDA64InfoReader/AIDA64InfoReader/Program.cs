using System;
using System.Windows.Forms;

namespace AIDA64InfoReader
{
    static class Program
    {
        /// <summary>
        /// 应用程序的主入口点。
        /// </summary>
        [STAThread]
        static void Main(String[] args)
        {
            
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Form1 form1 = new Form1(args);
            Application.Run(form1);
        }
    }
}
