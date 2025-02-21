using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Xml.Linq;
using Microsoft.Win32;
using System.IO.MemoryMappedFiles;
using System.IO.Ports;

namespace AIDA64InfoReader
{
    public partial class Form1 : Form
    {
        public struct AIDA64Item
        {
            internal string id;
            internal string value;
        }

        private String[] args;
        private IList<string> portList = new List<string>();
        private string portName;
        private const string defaultNullPort = "";

        System.Timers.Timer t = new System.Timers.Timer();

        public Form1(String[] args)
        {
            this.args = args;
            
            InitializeComponent();
            InitPort(false);
            InitComboBox();
            InitTimer();
        }

        private void InitComboBox()
        {
            comboBoxPortSelect.SelectedItem = portName;
        }

        /*
         * 初始化定时器
         */
        private void InitTimer()
        {
            textSendInterval.Value = 1000;//默认值为1000毫秒
            t.Interval = 1000;//实例化Timer类，设置默认间隔时间为1000毫秒
            t.Elapsed += ReadInfoAndSend;//到达时间的时候执行事件
            t.AutoReset = true;//设置是执行一次（false）还是一直执行(true
            t.SynchronizingObject = this;
            t.Start();
        }

        /*
         * 初始化端口
         */
        private void InitPort(bool isManualSelect)
        {
            portList.Add(defaultNullPort);//添加一个空选项
            //获取串口列表
            RegistryKey keyCom = Registry.LocalMachine.OpenSubKey("Hardware\\DeviceMap\\SerialComm");
            if (keyCom != null)
            {
                string[] sSubKeys = keyCom.GetValueNames();
                foreach (string sName in sSubKeys)
                {
                    portList.Add((string)keyCom.GetValue(sName));
                }
            }
            comboBoxPortSelect.DataSource = portList;
            if (args.Length > 1)
            {
                Console.WriteLine("参数只能为一个并且是串口名称");
                DialogResult dr = MessageBox.Show("参数只能为一个并且是串口名称", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
                if (dr == DialogResult.OK)
                {
                    Application.Exit();
                }
            }
            else if (args.Length == 1)//如果传了一个参数
            {
                if (!portList.Contains(args[0]))//串口不在系统串口列表中
                {
                    Console.WriteLine("参数中的串口\"" + portName + "\"不存在于系统之中");
                    DialogResult dr = MessageBox.Show("参数中的串口\"" + portName + "\"不存在于系统之中", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    if(dr == DialogResult.OK)
                    {
                        Application.Exit();
                    }
                }
                else
                {
                    portName = args[0];
                }
            }
            else if (portList.Count == 2)//没传参数则使用串口列表中唯一的串口
            {
                foreach(string port in portList)
                {
                    if (port != null && !port.Equals(defaultNullPort))
                    {
                        portName = port;
                    }
                }
                Console.WriteLine("参数中未传入串口名称，使用系统中唯一的串口：" + this.portName);
                //MessageBox.Show("参数中未传入串口名称，使用系统中唯一的串口：", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            else if(portList.Count > 2)//端口列表大于1(有一个默认的空值)
            {
                Console.WriteLine("检测到有多个串口，请手动选择端口");
                if (!isManualSelect)
                {
                    MessageBox.Show("检测到有多个串口，请手动选择端口", "提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                portName = defaultNullPort;
                return;
            }else if(portList.Count == 1)
            {
                MessageBox.Show("检测到没有可用端口，请先插上设备后再试", "警告", MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }
        }

        private bool IsPortAvailable()
        {
            return (portName != null && !portName.Equals(""));//没有找到串口的话直接返回
        }

        //定时任务，用于将从AIDA64获取到的信息发送到指定的串口
        private void ReadInfoAndSend(object source, System.Timers.ElapsedEventArgs e)
        {
            string SerialSendData = string.Empty;
            if (!IsPortAvailable())//没有找到串口的话直接返回
            {
                return;
            }
            try
            {
                string tempString = string.Empty;
                tempString += "<AIDA64>";

                MemoryMappedFile mmf = MemoryMappedFile.OpenExisting("AIDA64_SensorValues");
                MemoryMappedViewAccessor accessor = mmf.CreateViewAccessor();
                tempString = tempString + "";
                for (int i = 0; i < accessor.Capacity; i++)
                {
                    tempString = tempString + (char)accessor.ReadByte(i);
                }
                tempString = tempString.Replace("\0", "");
                tempString = tempString + "";
                accessor.Dispose();
                mmf.Dispose();

                tempString += "</AIDA64>";
                XDocument aidaXML = XDocument.Parse(tempString);
                var sysElements = aidaXML.Element("AIDA64").Elements("sys");
                var tempElements = aidaXML.Element("AIDA64").Elements("temp");
                var fanElements = aidaXML.Element("AIDA64").Elements("fan");
                var dutyElements = aidaXML.Element("AIDA64").Elements("duty");
                var voltElements = aidaXML.Element("AIDA64").Elements("volt");
                var pwrElements = aidaXML.Element("AIDA64").Elements("pwr");

                

                List<AIDA64Item> items = new List<AIDA64Item>();
                
                foreach (var i in sysElements)
                {
                    //Console.WriteLine(i.Element("id").Value + "\t" + i.Element("label").Value + "\t" + i.Element("value").Value);
                    AIDA64Item item = new AIDA64Item();
                    //内存数据根据注册表顺序，从上往下读取
                    switch (i.Element("id").Value)
                    {
                        case "SCPUUTI":  //CPU使用率
                            item.id = i.Element("id").Value;
                            item.value = i.Element("value").Value;
                            break;
                        case "SCPUCLK":  //CPU的主频
                            item.id = i.Element("id").Value;
                            item.value = i.Element("value").Value;
                            break;
                        case "SGPU1UTI":  //GPU使用率
                            item.id = i.Element("id").Value;
                            item.value = i.Element("value").Value;
                            break;
                        case "SGPU1CLK":  //GPU的主频
                            item.id = i.Element("id").Value;
                            item.value = i.Element("value").Value;
                            break;
                        case "SMEMUTI":  //内存的使用率
                            item.id = i.Element("id").Value;
                            item.value = i.Element("value").Value;
                            break;
                        case "SNIC5ULRATE":  //网络的上传速率
                            item.id = i.Element("id").Value;
                            item.value = i.Element("value").Value;
                            break;
                        case "SNIC5DLRATE":  //网络的下载速率
                            item.id = i.Element("id").Value;
                            item.value = i.Element("value").Value;
                            break;
                        default:
                            break;
                    }
                    if (item.id != null)
                    {
                        items.Add(item);
                    }
                }
                foreach (var i in tempElements)
                {
                    //Console.WriteLine(i.Element("id").Value + "\t" + i.Element("label").Value + "\t" + i.Element("value").Value);
                    AIDA64Item item = new AIDA64Item();
                    switch (i.Element("id").Value)
                    {
                        case "TCPU":   //CPU温度
                            item.id = i.Element("id").Value;
                            item.value = i.Element("value").Value;
                            break;
                        case "TGPU1":   //GPU温度
                            item.id = i.Element("id").Value;
                            item.value = i.Element("value").Value;
                            break;
                        default:
                            break;
                    }
                    if (item.id != null)
                    {
                        items.Add(item);
                    }
                }
                foreach (var i in pwrElements)
                {
                    //Console.WriteLine(i.Element("id").Value + "\t" + i.Element("label").Value + "\t" + i.Element("value").Value);
                    AIDA64Item item = new AIDA64Item();
                    switch (i.Element("id").Value)
                    {
                        case "PCPUPKG":   //CPU的功耗
                            item.id = i.Element("id").Value;
                            item.value = i.Element("value").Value;
                            break;
                        case "PGPU1TDPP":   //GPU的功耗
                            item.id = i.Element("id").Value;
                            item.value = i.Element("value").Value;
                            break;
                        default:
                            break;
                    }
                    if (item.id != null)
                    {
                        items.Add(item);
                    }
                }
                foreach (var i in fanElements)
                {
                    //Console.WriteLine(i.Element("id").Value + "\t" + i.Element("label").Value + "\t" + i.Element("value").Value);
                    AIDA64Item item = new AIDA64Item();
                    switch (i.Element("id").Value)
                    {
                        case "FCPU":  //CPU散热风扇
                            item.id = i.Element("id").Value;
                            item.value = i.Element("value").Value;
                            break;
                        case "FGPU1":  //GPU散热风扇
                            item.id = i.Element("id").Value;
                            item.value = i.Element("value").Value;
                            break;
                        default:
                            break;
                    }
                    if (item.id != null)
                    {
                        items.Add(item);
                    }
                }

                //string updateCmd = string.Empty + "update";
                string updateCmd = string.Empty;
                //数据打包成json格式
                foreach (var i in items)
                {
                    updateCmd += ("\"" + i.id + "\":\"" + i.value + "\",");
                }
                updateCmd = updateCmd.Substring(0, (updateCmd.Length-1)); //移除最后一个逗号
                updateCmd = "{" + updateCmd + "}";
                Console.Write(updateCmd);
                this.textCmdBox.Text = updateCmd;//这里的this指针在设置上面的t.SynchronizingObject = this;
                writeDataToPort(updateCmd);
            }
            catch(Exception ex)
            {
                handleException(ex);
            }
            
        }
        private void handleException(Exception ex)
        {
            Console.Write(ex.Message + ",可能已经拔下串口设备或者串口被占用或者发生其他异常");
            //MessageBox.Show(ex.Message + ",可能已经拔下串口设备或者串口被占用或者发生其他异常", "发生异常", MessageBoxButtons.OK, MessageBoxIcon.Error);
            //this.portName = null;
        }
        private void writeDataToPort(string dataStr)
        {
            SerialPort serialPort1 = new SerialPort(portName, 115200, Parity.None, 8, StopBits.One); //先到设备管理器里找串口对应的端口
            serialPort1.Open();
            serialPort1.Write(dataStr);
            serialPort1.Close();
        } 
        private void btnMinimize_Click(object sender, EventArgs e)
        {
            hideWindow();
        }

        //手动发送命令到指定的串口
        /*private void btnSendCmd_Click(object sender, EventArgs e)
        {
            if (t.Enabled)
            {
                MessageBox.Show("请先停止自动发送", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            try
            {
                if (textCmdBox.Text != null && !textCmdBox.Text.Equals(""))
                {
                    writeDataToPort(textCmdBox.Text);
                }
            }
            catch(Exception ex)
            {
                handleException(ex);
            }
        }
        */
        private void Form1_SizeChanged(object sender, EventArgs e)
        {
            //hideWindow();
        }

        private void notifyIcon1_MouseClick(object sender, MouseEventArgs e)
        {
            if(e.Button == MouseButtons.Left)//鼠标左键单击托盘图标才显示窗口
            {
                showWindow();
            }
        }

        private void hideWindow()
        {
            //首先将窗口最小化
            WindowState = FormWindowState.Minimized;
            //托盘显示图标等于托盘图标对象 
            //注意notifyIcon1是控件的名字而不是对象的名字
            //隐藏任务栏区图标 
            this.ShowInTaskbar = false;
            //图标显示在托盘区 
            notifyIcon1.Visible = true;   
        }

        private void showWindow()
        {
            //判断是否已经最小化于托盘 
            if (WindowState == FormWindowState.Minimized)
            {
                //还原窗体显示 
                WindowState = FormWindowState.Normal;
                //激活窗体并给予它焦点 
                this.Activate();
                //任务栏区显示图标 
                this.ShowInTaskbar = true;
                //托盘区图标隐藏 
                //notifyIcon1.Visible = false;
            }
            else if(WindowState == FormWindowState.Normal)//若窗口已经是正常显示状态，则直接使窗口获得焦点
            {
                this.Activate();
            }
        }

        private void 退出ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void btnStartAutoSend_Click(object sender, EventArgs e)
        {
            if (!t.Enabled)
            {
                if (t.Interval < 500 || t.Interval > 10000)
                {
                    MessageBox.Show("发送间隔不能小于500,也不能大于10000，太快也没用，AIDA64反正都是间隔一秒传数据", "输入错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
                else
                {
                    t.Interval = (double)textSendInterval.Value;
                    t.Start();
                }
            }
        }

        private void btnStopAutoSend_Click(object sender, EventArgs e)
        {
            if (t.Enabled)
            {
                t.Stop();
            }
        }

        private void textSendInterval_ValueChanged(object sender, EventArgs e)
        {
            t.Interval = (double)textSendInterval.Value;
        }

        private void comboBoxPortSelect_DropDown(object sender, EventArgs e)
        {
            InitPort(true);
        }

        private void comboBoxPortSelect_SelectedIndexChanged(object sender, EventArgs e)
        {
            portName = (string)comboBoxPortSelect.SelectedItem;
        }

        private void 关于ToolStripMenuItem_Click_1(object sender, EventArgs e)
        {
            MessageBox.Show("由Lan进行二次开发，源码摘自https://gitee.com/optimouskiller/AIDA64InfoReader\n个人QQ：522076290", "关于", MessageBoxButtons.OK, MessageBoxIcon.None);
        }

        private void 如何使用ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MessageBox.Show("1、打开AIDA64，依次点击“文件-设置”\n2、弹出的窗口中找到“外部程序”\n3、点击“全选”选中所有监测数据\n4、勾选“允许共享内存”\n5、最后点击“应用”并“OK”\n6、选择端口后自动执行", "如何使用", MessageBoxButtons.OK, MessageBoxIcon.None);
        }

        private void notifyIcon1_MouseDoubleClick(object sender, MouseEventArgs e)
        {

        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }
    }
}
