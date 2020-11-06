using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;


namespace FDMS_Ground_Controller
{
    public partial class Form1 : Form
    {
        TcpClient client;
        byte[] buf = new byte[10000];
        Thread th;
        sqlConnection newConnect;


        public Form1()
        {
            InitializeComponent();
            client = new TcpClient();

            dataGridView1.Columns.Add("Live", "Live Telemetry Data");
            dataGridView1.Columns[0].AutoSizeMode = DataGridViewAutoSizeColumnMode.Fill;

            //dataGridView2.Columns.Add("tailNumber", "Tail Number");
            //dataGridView2.Columns.Add("timeStamp", "Time Stamp");
            //dataGridView2.Columns.Add("accelx", "Accel-X");
            //dataGridView2.Columns.Add("accely", "Accel-Y");
            //dataGridView2.Columns.Add("accelz", "Accel-Z");
            //dataGridView2.Columns.Add("weight", "Weight");
            //dataGridView2.Columns.Add("altitude", "Altitude");
            //dataGridView2.Columns.Add("pitch", "Pitch");
            //dataGridView2.Columns.Add("bank", "Bank");
            
        }

        protected override void OnShown(EventArgs e)
        {
            base.OnShown(e);

            th = new Thread(new ThreadStart(tryConnect));
            th.IsBackground = true;
            th.Start();
        }

        public void tryConnect()
        {
            try
            {
                client.Connect("127.0.0.1", 54000);
                newConnect = new sqlConnection();
                newConnect.Initialize();
                client.GetStream().BeginRead(buf, 0, buf.Length, messageReceived, null);
                
            }
            catch
            {
                Thread.Sleep(500);
                tryConnect();
            }
        }

        private void messageReceived(IAsyncResult res)
        {
            
            if (res.IsCompleted)
            {

                try
                {
                    var bytesInput = client.GetStream().EndRead(res);


                    if (bytesInput > 0)
                    {
                        var tmp = new byte[bytesInput];
                        Array.Copy(buf, 0, tmp, 0, bytesInput);
                        var str = Encoding.ASCII.GetString(tmp);

                        if (label2.Visible == true)
                        {
                            BeginInvoke((Action)(() =>
                            {
                                dataGridView1.Rows.Add(str);
                            }));
                            newConnect.insert(str);
                        }
                        else
                        {

                        }
                    }
                }
                catch
                {
                    
                }
                try
                {
                    Array.Clear(buf, 0, buf.Length);
                    client.GetStream().BeginRead(buf, 0, buf.Length, messageReceived, null);
                }
                catch
                {
                    MessageBox.Show("Client Disconnected");
                    client.Close();
                    tryConnect();
                }
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            label3.Hide();
            textBox1.Hide();
            button3.Hide();
            dataGridView2.Hide();
            dataGridView1.Visible = true;

            if (label2.Visible == true)
            {
                label2.Hide();
            }
            else
            {
                label2.Visible = Visible;
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            label3.Visible = true;
            textBox1.Visible = true;
            button3.Visible = true;
            dataGridView2.Visible = true;
            dataGridView1.Hide();
            label2.Hide();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            string tailNum = textBox1.Text;
            sqlConnection tempSearch = new sqlConnection();
            tempSearch.Initialize();
            DataSet ds = tempSearch.select(tailNum);
            dataGridView2.ReadOnly = true;
            dataGridView2.DataSource = ds.Tables[0];
        }
    }
}
