using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;

namespace QuadcopterControlSerialPort
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            serialPort1.Open();
            serialPort1.DataReceived += new SerialDataReceivedEventHandler(DataReceivedHandler);
        }
        void SendASCIItoSerialPort(int ASCII)
        {
            byte[] buffer = new byte[] { Convert.ToByte(ASCII) };
            serialPort1.Write(buffer,0,1);
        }
         protected override void OnKeyDown(KeyEventArgs e)
        {
             base.OnKeyDown(e);
             SendASCIItoSerialPort(e.KeyValue);
        }
        delegate void SetTextCallback(string text);
        private void SetText(string text)
        {

            if (this.label1.InvokeRequired)
            {
                SetTextCallback d = new SetTextCallback(SetText);
                this.Invoke(d, new object[] { text });
            }
            else
            {
                this.label1.Text = text;
            }
        }
        private void SetText1(string text)
        {

            if (this.label5.InvokeRequired)
            {
                SetTextCallback d = new SetTextCallback(SetText1);
                this.Invoke(d, new object[] { text });
            }
            else
            {
                this.label5.Text = text;
            }
        }
        private void SetText2(string text)
        {

            if (this.label6.InvokeRequired)
            {
                SetTextCallback d = new SetTextCallback(SetText2);
                this.Invoke(d, new object[] { text });
            }
            else
            {
                this.label6.Text = text;
            }
        }
        private void SetText3(string text)
        {

            if (this.label9.InvokeRequired)
            {
                SetTextCallback d = new SetTextCallback(SetText3);
                this.Invoke(d, new object[] { text });
            }
            else
            {
                this.label9.Text = text;
            }
        }
        private void DataReceivedHandler( object sender, SerialDataReceivedEventArgs e)
        {
            string message = serialPort1.ReadLine();
            SetText(message.ToString());
            string[] parameters= new string[9];
            parameters = message.Split(',');
            if(parameters.Length>8)
            {
                SetText1(parameters[1]);
                SetText2(parameters[4]);
                //SetText3(parameters[2]);
                SetText3(parameters[6]);
            }
        }

    }
}
