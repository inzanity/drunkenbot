using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using System.IO;
using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;

using System.Runtime.InteropServices;

namespace ParticleEditor
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class Form1 : System.Windows.Forms.Form
	{
		bool dirty = false;
		private bool Dirty
		{
			get
			{
				return dirty;
			}
			set
			{
				if (dirty != value)
				{
					if (value) Text += '*';
					else Text = Text.TrimEnd("*".ToCharArray());
				}
				dirty = value;
			}
		}
		private const float ROT_CHANGE = .04f;
		private const float DIST_CHANGE = 1.0f;
		private ParticleLib.D3DObj d3dObj;
		private ParticleLib.ParticleSystem particleSystem;
		private System.Timers.Timer timer1;
		private int time = 0;
		private float xRot, yRot, dist;
		private bool[] buttonPressed = new bool[6];
		private bool resized = false;
		private bool play = true;
		private String homeDir = Directory.GetCurrentDirectory();

		private System.Windows.Forms.StatusBar statusBar1;
		private System.Windows.Forms.Panel panel1;
		private System.Windows.Forms.MainMenu mainMenu1;
		private System.Windows.Forms.MenuItem menuItemFile;
		private System.Windows.Forms.MenuItem menuItemNew;
		private System.Windows.Forms.MenuItem menuItemOpen;
		private System.Windows.Forms.MenuItem menuItemSave;
		private System.Windows.Forms.MenuItem menuItemSaveAs;
		private System.Windows.Forms.MenuItem menuItemExit;
		private System.Windows.Forms.TextBox textBox1;
		private System.Windows.Forms.ImageList imageList1;
		private System.Windows.Forms.Button buttonLeft;
		private System.Windows.Forms.Button buttonUp;
		private System.Windows.Forms.Button buttonRight;
		private System.Windows.Forms.Button buttonDown;
		private System.Windows.Forms.Button buttonNear;
		private System.Windows.Forms.Button buttonFar;
		private System.Windows.Forms.Panel panel2;
		private System.Windows.Forms.Splitter splitter1;
		private System.Windows.Forms.Button buttonReload;
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.GroupBox groupBox2;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.Label label5;
		private System.Windows.Forms.OpenFileDialog openFileDialogPS;
		private System.Windows.Forms.OpenFileDialog openFileDialogTex;
		private System.Windows.Forms.ColorDialog colorDialog1;
		private System.Windows.Forms.SaveFileDialog saveFileDialogPS;
		private System.Windows.Forms.GroupBox groupBox3;
		private System.Windows.Forms.TextBox textBoxColorRGB;
		private System.Windows.Forms.TextBox textBoxColorInt;
		private System.Windows.Forms.Button buttonColor;
		private System.Windows.Forms.CheckBox checkBoxLooping;
		private System.Windows.Forms.PictureBox pictureBoxTexture;
		private System.Windows.Forms.NumericUpDown numericUpDownParticles;
		private System.Windows.Forms.NumericUpDown numericUpDownSizes;
		private System.Windows.Forms.NumericUpDown numericUpDownColors;
		private System.Windows.Forms.NumericUpDown numericUpDownCoordinates;
		private System.Windows.Forms.CheckBox checkBox1;
		private System.Windows.Forms.Label label6;
		private System.Windows.Forms.Label label7;
		private System.Windows.Forms.Label label8;
		private System.Windows.Forms.Button buttonPlay;
		private System.Windows.Forms.TrackBar trackBarSpeed;
		private System.ComponentModel.IContainer components;

		public Form1()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			d3dObj = new ParticleLib.D3DObj((int)panel1.Handle);
			d3dObj.initWindowed();
			menuItemNew_Click(null, null);

			for (int i = 0; i < 6; i++)
				buttonPressed[i] = false;
			timer1.Start();
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			timer1.Stop();
			if( disposing )
			{
				if (components != null) 
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.components = new System.ComponentModel.Container();
			System.Resources.ResourceManager resources = new System.Resources.ResourceManager(typeof(Form1));
			this.statusBar1 = new System.Windows.Forms.StatusBar();
			this.panel1 = new System.Windows.Forms.Panel();
			this.mainMenu1 = new System.Windows.Forms.MainMenu();
			this.menuItemFile = new System.Windows.Forms.MenuItem();
			this.menuItemNew = new System.Windows.Forms.MenuItem();
			this.menuItemOpen = new System.Windows.Forms.MenuItem();
			this.menuItemSave = new System.Windows.Forms.MenuItem();
			this.menuItemSaveAs = new System.Windows.Forms.MenuItem();
			this.menuItemExit = new System.Windows.Forms.MenuItem();
			this.textBox1 = new System.Windows.Forms.TextBox();
			this.timer1 = new System.Timers.Timer();
			this.imageList1 = new System.Windows.Forms.ImageList(this.components);
			this.buttonLeft = new System.Windows.Forms.Button();
			this.buttonUp = new System.Windows.Forms.Button();
			this.buttonRight = new System.Windows.Forms.Button();
			this.buttonDown = new System.Windows.Forms.Button();
			this.buttonNear = new System.Windows.Forms.Button();
			this.buttonFar = new System.Windows.Forms.Button();
			this.panel2 = new System.Windows.Forms.Panel();
			this.groupBox2 = new System.Windows.Forms.GroupBox();
			this.label5 = new System.Windows.Forms.Label();
			this.checkBoxLooping = new System.Windows.Forms.CheckBox();
			this.pictureBoxTexture = new System.Windows.Forms.PictureBox();
			this.label4 = new System.Windows.Forms.Label();
			this.numericUpDownParticles = new System.Windows.Forms.NumericUpDown();
			this.label3 = new System.Windows.Forms.Label();
			this.numericUpDownSizes = new System.Windows.Forms.NumericUpDown();
			this.label2 = new System.Windows.Forms.Label();
			this.numericUpDownColors = new System.Windows.Forms.NumericUpDown();
			this.label1 = new System.Windows.Forms.Label();
			this.numericUpDownCoordinates = new System.Windows.Forms.NumericUpDown();
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.label8 = new System.Windows.Forms.Label();
			this.label7 = new System.Windows.Forms.Label();
			this.label6 = new System.Windows.Forms.Label();
			this.checkBox1 = new System.Windows.Forms.CheckBox();
			this.buttonPlay = new System.Windows.Forms.Button();
			this.trackBarSpeed = new System.Windows.Forms.TrackBar();
			this.buttonReload = new System.Windows.Forms.Button();
			this.groupBox3 = new System.Windows.Forms.GroupBox();
			this.textBoxColorRGB = new System.Windows.Forms.TextBox();
			this.buttonColor = new System.Windows.Forms.Button();
			this.textBoxColorInt = new System.Windows.Forms.TextBox();
			this.splitter1 = new System.Windows.Forms.Splitter();
			this.openFileDialogPS = new System.Windows.Forms.OpenFileDialog();
			this.openFileDialogTex = new System.Windows.Forms.OpenFileDialog();
			this.colorDialog1 = new System.Windows.Forms.ColorDialog();
			this.saveFileDialogPS = new System.Windows.Forms.SaveFileDialog();
			((System.ComponentModel.ISupportInitialize)(this.timer1)).BeginInit();
			this.panel2.SuspendLayout();
			this.groupBox2.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.numericUpDownParticles)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.numericUpDownSizes)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.numericUpDownColors)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.numericUpDownCoordinates)).BeginInit();
			this.groupBox1.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.trackBarSpeed)).BeginInit();
			this.groupBox3.SuspendLayout();
			this.SuspendLayout();
			// 
			// statusBar1
			// 
			this.statusBar1.Location = new System.Drawing.Point(0, 537);
			this.statusBar1.Name = "statusBar1";
			this.statusBar1.Size = new System.Drawing.Size(596, 22);
			this.statusBar1.TabIndex = 1;
			this.statusBar1.Text = "Ready";
			// 
			// panel1
			// 
			this.panel1.Anchor = (((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right);
			this.panel1.BackColor = System.Drawing.Color.Black;
			this.panel1.Location = new System.Drawing.Point(8, 8);
			this.panel1.Name = "panel1";
			this.panel1.Size = new System.Drawing.Size(306, 340);
			this.panel1.TabIndex = 2;
			this.panel1.Resize += new System.EventHandler(this.Panel1_Resize);
			// 
			// mainMenu1
			// 
			this.mainMenu1.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																					  this.menuItemFile});
			// 
			// menuItemFile
			// 
			this.menuItemFile.Index = 0;
			this.menuItemFile.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																						 this.menuItemNew,
																						 this.menuItemOpen,
																						 this.menuItemSave,
																						 this.menuItemSaveAs,
																						 this.menuItemExit});
			this.menuItemFile.Text = "&File";
			// 
			// menuItemNew
			// 
			this.menuItemNew.Index = 0;
			this.menuItemNew.Shortcut = System.Windows.Forms.Shortcut.CtrlN;
			this.menuItemNew.Text = "&New";
			this.menuItemNew.Click += new System.EventHandler(this.menuItemNew_Click);
			// 
			// menuItemOpen
			// 
			this.menuItemOpen.Index = 1;
			this.menuItemOpen.Shortcut = System.Windows.Forms.Shortcut.CtrlO;
			this.menuItemOpen.Text = "&Open...";
			this.menuItemOpen.Click += new System.EventHandler(this.menuItemOpen_Click);
			// 
			// menuItemSave
			// 
			this.menuItemSave.Index = 2;
			this.menuItemSave.Shortcut = System.Windows.Forms.Shortcut.CtrlS;
			this.menuItemSave.Text = "&Save";
			this.menuItemSave.Click += new System.EventHandler(this.menuItemSave_Click);
			// 
			// menuItemSaveAs
			// 
			this.menuItemSaveAs.Index = 3;
			this.menuItemSaveAs.Text = "Save &as...";
			this.menuItemSaveAs.Click += new System.EventHandler(this.menuItemSave_Click);
			// 
			// menuItemExit
			// 
			this.menuItemExit.Index = 4;
			this.menuItemExit.Text = "E&xit";
			this.menuItemExit.Click += new System.EventHandler(this.menuItemExit_Click);
			// 
			// textBox1
			// 
			this.textBox1.AcceptsReturn = true;
			this.textBox1.AcceptsTab = true;
			this.textBox1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.textBox1.Location = new System.Drawing.Point(0, 348);
			this.textBox1.Multiline = true;
			this.textBox1.Name = "textBox1";
			this.textBox1.ScrollBars = System.Windows.Forms.ScrollBars.Both;
			this.textBox1.Size = new System.Drawing.Size(596, 189);
			this.textBox1.TabIndex = 0;
			this.textBox1.Text = "";
			this.textBox1.TextChanged += new System.EventHandler(this.valueChanged);
			// 
			// timer1
			// 
			this.timer1.Interval = 25;
			this.timer1.SynchronizingObject = this;
			this.timer1.Elapsed += new System.Timers.ElapsedEventHandler(this.timer1_Elapsed);
			// 
			// imageList1
			// 
			this.imageList1.ColorDepth = System.Windows.Forms.ColorDepth.Depth24Bit;
			this.imageList1.ImageSize = new System.Drawing.Size(16, 16);
			this.imageList1.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageList1.ImageStream")));
			this.imageList1.TransparentColor = System.Drawing.Color.Teal;
			// 
			// buttonLeft
			// 
			this.buttonLeft.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.buttonLeft.Image = ((System.Drawing.Bitmap)(resources.GetObject("buttonLeft.Image")));
			this.buttonLeft.ImageIndex = 0;
			this.buttonLeft.ImageList = this.imageList1;
			this.buttonLeft.Location = new System.Drawing.Point(16, 40);
			this.buttonLeft.Name = "buttonLeft";
			this.buttonLeft.Size = new System.Drawing.Size(24, 32);
			this.buttonLeft.TabIndex = 3;
			this.buttonLeft.MouseUp += new System.Windows.Forms.MouseEventHandler(this.button_MouseUp);
			this.buttonLeft.KeyUp += new System.Windows.Forms.KeyEventHandler(this.button_KeyUp);
			this.buttonLeft.KeyDown += new System.Windows.Forms.KeyEventHandler(this.button_KeyDown);
			this.buttonLeft.MouseDown += new System.Windows.Forms.MouseEventHandler(this.button_MouseDown);
			// 
			// buttonUp
			// 
			this.buttonUp.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.buttonUp.Image = ((System.Drawing.Bitmap)(resources.GetObject("buttonUp.Image")));
			this.buttonUp.ImageIndex = 2;
			this.buttonUp.ImageList = this.imageList1;
			this.buttonUp.Location = new System.Drawing.Point(40, 16);
			this.buttonUp.Name = "buttonUp";
			this.buttonUp.Size = new System.Drawing.Size(32, 24);
			this.buttonUp.TabIndex = 4;
			this.buttonUp.MouseUp += new System.Windows.Forms.MouseEventHandler(this.button_MouseUp);
			this.buttonUp.KeyUp += new System.Windows.Forms.KeyEventHandler(this.button_KeyUp);
			this.buttonUp.KeyDown += new System.Windows.Forms.KeyEventHandler(this.button_KeyDown);
			this.buttonUp.MouseDown += new System.Windows.Forms.MouseEventHandler(this.button_MouseDown);
			// 
			// buttonRight
			// 
			this.buttonRight.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.buttonRight.Image = ((System.Drawing.Bitmap)(resources.GetObject("buttonRight.Image")));
			this.buttonRight.ImageIndex = 1;
			this.buttonRight.ImageList = this.imageList1;
			this.buttonRight.Location = new System.Drawing.Point(72, 40);
			this.buttonRight.Name = "buttonRight";
			this.buttonRight.Size = new System.Drawing.Size(24, 32);
			this.buttonRight.TabIndex = 5;
			this.buttonRight.MouseUp += new System.Windows.Forms.MouseEventHandler(this.button_MouseUp);
			this.buttonRight.KeyUp += new System.Windows.Forms.KeyEventHandler(this.button_KeyUp);
			this.buttonRight.KeyDown += new System.Windows.Forms.KeyEventHandler(this.button_KeyDown);
			this.buttonRight.MouseDown += new System.Windows.Forms.MouseEventHandler(this.button_MouseDown);
			// 
			// buttonDown
			// 
			this.buttonDown.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.buttonDown.Image = ((System.Drawing.Bitmap)(resources.GetObject("buttonDown.Image")));
			this.buttonDown.ImageIndex = 3;
			this.buttonDown.ImageList = this.imageList1;
			this.buttonDown.Location = new System.Drawing.Point(40, 72);
			this.buttonDown.Name = "buttonDown";
			this.buttonDown.Size = new System.Drawing.Size(32, 24);
			this.buttonDown.TabIndex = 6;
			this.buttonDown.MouseUp += new System.Windows.Forms.MouseEventHandler(this.button_MouseUp);
			this.buttonDown.KeyUp += new System.Windows.Forms.KeyEventHandler(this.button_KeyUp);
			this.buttonDown.KeyDown += new System.Windows.Forms.KeyEventHandler(this.button_KeyDown);
			this.buttonDown.MouseDown += new System.Windows.Forms.MouseEventHandler(this.button_MouseDown);
			// 
			// buttonNear
			// 
			this.buttonNear.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.buttonNear.Image = ((System.Drawing.Bitmap)(resources.GetObject("buttonNear.Image")));
			this.buttonNear.ImageIndex = 4;
			this.buttonNear.ImageList = this.imageList1;
			this.buttonNear.Location = new System.Drawing.Point(40, 104);
			this.buttonNear.Name = "buttonNear";
			this.buttonNear.Size = new System.Drawing.Size(32, 24);
			this.buttonNear.TabIndex = 7;
			this.buttonNear.MouseUp += new System.Windows.Forms.MouseEventHandler(this.button_MouseUp);
			this.buttonNear.KeyUp += new System.Windows.Forms.KeyEventHandler(this.button_KeyUp);
			this.buttonNear.KeyDown += new System.Windows.Forms.KeyEventHandler(this.button_KeyDown);
			this.buttonNear.MouseDown += new System.Windows.Forms.MouseEventHandler(this.button_MouseDown);
			// 
			// buttonFar
			// 
			this.buttonFar.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.buttonFar.Image = ((System.Drawing.Bitmap)(resources.GetObject("buttonFar.Image")));
			this.buttonFar.ImageIndex = 5;
			this.buttonFar.ImageList = this.imageList1;
			this.buttonFar.Location = new System.Drawing.Point(40, 128);
			this.buttonFar.Name = "buttonFar";
			this.buttonFar.Size = new System.Drawing.Size(32, 24);
			this.buttonFar.TabIndex = 8;
			this.buttonFar.MouseUp += new System.Windows.Forms.MouseEventHandler(this.button_MouseUp);
			this.buttonFar.KeyUp += new System.Windows.Forms.KeyEventHandler(this.button_KeyUp);
			this.buttonFar.KeyDown += new System.Windows.Forms.KeyEventHandler(this.button_KeyDown);
			this.buttonFar.MouseDown += new System.Windows.Forms.MouseEventHandler(this.button_MouseDown);
			// 
			// panel2
			// 
			this.panel2.Controls.AddRange(new System.Windows.Forms.Control[] {
																				 this.groupBox2,
																				 this.panel1,
																				 this.groupBox1,
																				 this.groupBox3});
			this.panel2.Dock = System.Windows.Forms.DockStyle.Top;
			this.panel2.Name = "panel2";
			this.panel2.Size = new System.Drawing.Size(596, 340);
			this.panel2.TabIndex = 9;
			// 
			// groupBox2
			// 
			this.groupBox2.Anchor = (System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right);
			this.groupBox2.Controls.AddRange(new System.Windows.Forms.Control[] {
																					this.label5,
																					this.checkBoxLooping,
																					this.pictureBoxTexture,
																					this.label4,
																					this.numericUpDownParticles,
																					this.label3,
																					this.numericUpDownSizes,
																					this.label2,
																					this.numericUpDownColors,
																					this.label1,
																					this.numericUpDownCoordinates});
			this.groupBox2.Location = new System.Drawing.Point(447, 5);
			this.groupBox2.Name = "groupBox2";
			this.groupBox2.Size = new System.Drawing.Size(140, 241);
			this.groupBox2.TabIndex = 11;
			this.groupBox2.TabStop = false;
			this.groupBox2.Text = "Properties";
			// 
			// label5
			// 
			this.label5.Location = new System.Drawing.Point(13, 165);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(43, 18);
			this.label5.TabIndex = 10;
			this.label5.Text = "Texture";
			// 
			// checkBoxLooping
			// 
			this.checkBoxLooping.Location = new System.Drawing.Point(60, 136);
			this.checkBoxLooping.Name = "checkBoxLooping";
			this.checkBoxLooping.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
			this.checkBoxLooping.Size = new System.Drawing.Size(64, 24);
			this.checkBoxLooping.TabIndex = 9;
			this.checkBoxLooping.Text = "Looping";
			this.checkBoxLooping.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			this.checkBoxLooping.CheckedChanged += new System.EventHandler(this.valueChanged);
			// 
			// pictureBoxTexture
			// 
			this.pictureBoxTexture.BackColor = System.Drawing.SystemColors.Control;
			this.pictureBoxTexture.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.pictureBoxTexture.Location = new System.Drawing.Point(65, 166);
			this.pictureBoxTexture.Name = "pictureBoxTexture";
			this.pictureBoxTexture.Size = new System.Drawing.Size(64, 64);
			this.pictureBoxTexture.SizeMode = System.Windows.Forms.PictureBoxSizeMode.CenterImage;
			this.pictureBoxTexture.TabIndex = 8;
			this.pictureBoxTexture.TabStop = false;
			this.pictureBoxTexture.Click += new System.EventHandler(this.pictureBoxTexture_Click);
			// 
			// label4
			// 
			this.label4.Location = new System.Drawing.Point(27, 28);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(51, 17);
			this.label4.TabIndex = 7;
			this.label4.Text = "Particles";
			// 
			// numericUpDownParticles
			// 
			this.numericUpDownParticles.Location = new System.Drawing.Point(83, 24);
			this.numericUpDownParticles.Name = "numericUpDownParticles";
			this.numericUpDownParticles.Size = new System.Drawing.Size(46, 20);
			this.numericUpDownParticles.TabIndex = 6;
			this.numericUpDownParticles.Value = new System.Decimal(new int[] {
																				 100,
																				 0,
																				 0,
																				 0});
			this.numericUpDownParticles.TextChanged += new System.EventHandler(this.valueChanged);
			// 
			// label3
			// 
			this.label3.Location = new System.Drawing.Point(39, 108);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(38, 17);
			this.label3.TabIndex = 5;
			this.label3.Text = "Sizes";
			// 
			// numericUpDownSizes
			// 
			this.numericUpDownSizes.Location = new System.Drawing.Point(82, 108);
			this.numericUpDownSizes.Name = "numericUpDownSizes";
			this.numericUpDownSizes.Size = new System.Drawing.Size(47, 20);
			this.numericUpDownSizes.TabIndex = 4;
			this.numericUpDownSizes.Value = new System.Decimal(new int[] {
																			 1,
																			 0,
																			 0,
																			 0});
			this.numericUpDownSizes.TextChanged += new System.EventHandler(this.valueChanged);
			// 
			// label2
			// 
			this.label2.Location = new System.Drawing.Point(36, 80);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(41, 21);
			this.label2.TabIndex = 3;
			this.label2.Text = "Colors";
			// 
			// numericUpDownColors
			// 
			this.numericUpDownColors.Location = new System.Drawing.Point(82, 80);
			this.numericUpDownColors.Name = "numericUpDownColors";
			this.numericUpDownColors.Size = new System.Drawing.Size(47, 20);
			this.numericUpDownColors.TabIndex = 2;
			this.numericUpDownColors.Value = new System.Decimal(new int[] {
																			  1,
																			  0,
																			  0,
																			  0});
			this.numericUpDownColors.TextChanged += new System.EventHandler(this.valueChanged);
			// 
			// label1
			// 
			this.label1.Location = new System.Drawing.Point(8, 56);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(68, 18);
			this.label1.TabIndex = 1;
			this.label1.Text = "Coordinates";
			// 
			// numericUpDownCoordinates
			// 
			this.numericUpDownCoordinates.Location = new System.Drawing.Point(81, 52);
			this.numericUpDownCoordinates.Name = "numericUpDownCoordinates";
			this.numericUpDownCoordinates.Size = new System.Drawing.Size(48, 20);
			this.numericUpDownCoordinates.TabIndex = 0;
			this.numericUpDownCoordinates.Value = new System.Decimal(new int[] {
																				   2,
																				   0,
																				   0,
																				   0});
			this.numericUpDownCoordinates.TextChanged += new System.EventHandler(this.valueChanged);
			// 
			// groupBox1
			// 
			this.groupBox1.Anchor = (System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right);
			this.groupBox1.Controls.AddRange(new System.Windows.Forms.Control[] {
																					this.label8,
																					this.label7,
																					this.label6,
																					this.checkBox1,
																					this.buttonPlay,
																					this.trackBarSpeed,
																					this.buttonReload,
																					this.buttonUp,
																					this.buttonFar,
																					this.buttonDown,
																					this.buttonRight,
																					this.buttonNear,
																					this.buttonLeft});
			this.groupBox1.Location = new System.Drawing.Point(325, 3);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(112, 327);
			this.groupBox1.TabIndex = 10;
			this.groupBox1.TabStop = false;
			// 
			// label8
			// 
			this.label8.Location = new System.Drawing.Point(69, 212);
			this.label8.Name = "label8";
			this.label8.Size = new System.Drawing.Size(33, 12);
			this.label8.TabIndex = 15;
			this.label8.Text = "200%";
			// 
			// label7
			// 
			this.label7.Location = new System.Drawing.Point(12, 211);
			this.label7.Name = "label7";
			this.label7.Size = new System.Drawing.Size(25, 11);
			this.label7.TabIndex = 14;
			this.label7.Text = "0%";
			// 
			// label6
			// 
			this.label6.Location = new System.Drawing.Point(8, 168);
			this.label6.Name = "label6";
			this.label6.Size = new System.Drawing.Size(44, 16);
			this.label6.TabIndex = 13;
			this.label6.Text = "Speed";
			// 
			// checkBox1
			// 
			this.checkBox1.Checked = true;
			this.checkBox1.CheckState = System.Windows.Forms.CheckState.Checked;
			this.checkBox1.Location = new System.Drawing.Point(48, 244);
			this.checkBox1.Name = "checkBox1";
			this.checkBox1.Size = new System.Drawing.Size(60, 16);
			this.checkBox1.TabIndex = 12;
			this.checkBox1.Text = "Replay";
			// 
			// buttonPlay
			// 
			this.buttonPlay.Image = ((System.Drawing.Bitmap)(resources.GetObject("buttonPlay.Image")));
			this.buttonPlay.ImageIndex = 7;
			this.buttonPlay.ImageList = this.imageList1;
			this.buttonPlay.Location = new System.Drawing.Point(12, 240);
			this.buttonPlay.Name = "buttonPlay";
			this.buttonPlay.Size = new System.Drawing.Size(28, 24);
			this.buttonPlay.TabIndex = 11;
			this.buttonPlay.Click += new System.EventHandler(this.buttonPlay_Click);
			// 
			// trackBarSpeed
			// 
			this.trackBarSpeed.Location = new System.Drawing.Point(8, 184);
			this.trackBarSpeed.Name = "trackBarSpeed";
			this.trackBarSpeed.Size = new System.Drawing.Size(96, 42);
			this.trackBarSpeed.TabIndex = 10;
			this.trackBarSpeed.Value = 5;
			// 
			// buttonReload
			// 
			this.buttonReload.Location = new System.Drawing.Point(16, 292);
			this.buttonReload.Name = "buttonReload";
			this.buttonReload.Size = new System.Drawing.Size(80, 24);
			this.buttonReload.TabIndex = 9;
			this.buttonReload.Text = "Reload";
			this.buttonReload.Click += new System.EventHandler(this.buttonReload_Click);
			// 
			// groupBox3
			// 
			this.groupBox3.Anchor = (System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right);
			this.groupBox3.Controls.AddRange(new System.Windows.Forms.Control[] {
																					this.textBoxColorRGB,
																					this.buttonColor,
																					this.textBoxColorInt});
			this.groupBox3.Location = new System.Drawing.Point(447, 251);
			this.groupBox3.Name = "groupBox3";
			this.groupBox3.Size = new System.Drawing.Size(140, 81);
			this.groupBox3.TabIndex = 13;
			this.groupBox3.TabStop = false;
			this.groupBox3.Text = "Colors";
			// 
			// textBoxColorRGB
			// 
			this.textBoxColorRGB.Location = new System.Drawing.Point(12, 19);
			this.textBoxColorRGB.Name = "textBoxColorRGB";
			this.textBoxColorRGB.ReadOnly = true;
			this.textBoxColorRGB.Size = new System.Drawing.Size(76, 20);
			this.textBoxColorRGB.TabIndex = 11;
			this.textBoxColorRGB.Text = "";
			// 
			// buttonColor
			// 
			this.buttonColor.Location = new System.Drawing.Point(102, 31);
			this.buttonColor.Name = "buttonColor";
			this.buttonColor.Size = new System.Drawing.Size(24, 24);
			this.buttonColor.TabIndex = 10;
			this.buttonColor.Click += new System.EventHandler(this.buttonColor_Click);
			// 
			// textBoxColorInt
			// 
			this.textBoxColorInt.Location = new System.Drawing.Point(12, 50);
			this.textBoxColorInt.Name = "textBoxColorInt";
			this.textBoxColorInt.ReadOnly = true;
			this.textBoxColorInt.Size = new System.Drawing.Size(76, 20);
			this.textBoxColorInt.TabIndex = 12;
			this.textBoxColorInt.Text = "";
			// 
			// splitter1
			// 
			this.splitter1.Dock = System.Windows.Forms.DockStyle.Top;
			this.splitter1.Location = new System.Drawing.Point(0, 340);
			this.splitter1.Name = "splitter1";
			this.splitter1.Size = new System.Drawing.Size(596, 8);
			this.splitter1.TabIndex = 10;
			this.splitter1.TabStop = false;
			// 
			// openFileDialogPS
			// 
			this.openFileDialogPS.Filter = "\"Lua files (*.lua)|*.lua|All files (*.*)|*.*\"";
			this.openFileDialogPS.InitialDirectory = "ParticleEffects";
			this.openFileDialogPS.Title = "Open Particle Effect";
			// 
			// openFileDialogTex
			// 
			this.openFileDialogTex.Filter = "\"Texture files (*.bmp)|*.bmp|All files (*.*)|*.*\"";
			this.openFileDialogTex.InitialDirectory = "textures";
			this.openFileDialogTex.Title = "Select texture";
			// 
			// saveFileDialogPS
			// 
			this.saveFileDialogPS.DefaultExt = "lua";
			this.saveFileDialogPS.Filter = "\"Lua files (*.lua)|*.lua|All files (*.*)|*.*\"";
			this.saveFileDialogPS.InitialDirectory = "ParticleEffects";
			this.saveFileDialogPS.Title = "Save Particle Effect";
			// 
			// Form1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(596, 559);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																		  this.textBox1,
																		  this.splitter1,
																		  this.statusBar1,
																		  this.panel2});
			this.Menu = this.mainMenu1;
			this.MinimumSize = new System.Drawing.Size(480, 400);
			this.Name = "Form1";
			this.Text = "Particle Editor";
			this.Closing += new System.ComponentModel.CancelEventHandler(this.Form1_Closing);
			((System.ComponentModel.ISupportInitialize)(this.timer1)).EndInit();
			this.panel2.ResumeLayout(false);
			this.groupBox2.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.numericUpDownParticles)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.numericUpDownSizes)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.numericUpDownColors)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.numericUpDownCoordinates)).EndInit();
			this.groupBox1.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.trackBarSpeed)).EndInit();
			this.groupBox3.ResumeLayout(false);
			this.ResumeLayout(false);

		}
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main() 
		{
			Application.Run(new Form1());
		}

		private void timer1_Elapsed(object sender, System.Timers.ElapsedEventArgs e)
		{
			if (resized)
			{
				if (panel1.Width <= 0 || panel1.Height <= 0)
					return;
				d3dObj.initWindowed();
//				particleSystem = new ParticleLib.ParticleSystem(100, 2, 1, 1, "particle.bmp");
				resized = false;
				return;
			}
			if (particleSystem == null)
				return;
			if (buttonPressed[0])
				yRot += ROT_CHANGE;
			if (buttonPressed[1])
				yRot -= ROT_CHANGE;
			if (buttonPressed[2])
				xRot += ROT_CHANGE;
			if (buttonPressed[3])
				xRot -= ROT_CHANGE;
			if (buttonPressed[4] && dist > 0)
				dist -= DIST_CHANGE;
			if (buttonPressed[5])
				dist += DIST_CHANGE;
			d3dObj.setTransform(xRot, yRot, dist);
			d3dObj.beginScene();
			particleSystem.draw(time);
			if (play)
				time += 2 * trackBarSpeed.Value * Convert.ToInt32(timer1.Interval) / trackBarSpeed.Maximum;
			d3dObj.endScene();
		}

		private void button_MouseDown(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			buttonPressed[((Button)sender).ImageIndex] = true;
		}
		private void button_MouseUp(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			buttonPressed[((Button)sender).ImageIndex] = false;
		}
		private void button_KeyDown(object sender, System.Windows.Forms.KeyEventArgs e)
		{
			if (e.KeyCode == Keys.Space || e.KeyCode == Keys.Enter)
				buttonPressed[((Button)sender).ImageIndex] = true;
		}
		private void button_KeyUp(object sender, System.Windows.Forms.KeyEventArgs e)
		{
			if (e.KeyCode == Keys.Space || e.KeyCode == Keys.Enter)
				buttonPressed[((Button)sender).ImageIndex] = false;
		}

		private void Panel1_Resize(object sender, System.EventArgs e)
		{
			particleSystem = null;
			statusBar1.Text = "Press \"Reload\" after resizing";
			resized = true;
		}

		private void pictureBoxTexture_Click(object sender, System.EventArgs e)
		{
			if (openFileDialogTex.ShowDialog() == DialogResult.OK)
			{
				pictureBoxTexture.Image = Image.FromFile(openFileDialogTex.FileName);
				particleSystem.setTexture(openFileDialogTex.FileName);
				Dirty = true;
			}
		}

		private void buttonColor_Click(object sender, System.EventArgs e)
		{
			if (colorDialog1.ShowDialog() == DialogResult.OK) 
			{
				buttonColor.BackColor = colorDialog1.Color;
				textBoxColorRGB.Text =	Convert.ToString(colorDialog1.Color.R) + ", " +
					Convert.ToString(colorDialog1.Color.G) + ", " +
					Convert.ToString(colorDialog1.Color.B);
				textBoxColorInt.Text = Convert.ToString(colorDialog1.Color.ToArgb());
			}
		}

		private void menuItemSave_Click(object sender, System.EventArgs e)
		{
			save(sender);
		}

		private bool save(object sender)
		{
			if (sender == menuItemSaveAs || saveFileDialogPS.FileName == "")
				if (saveFileDialogPS.ShowDialog() != DialogResult.OK)
					return false;
			FileStream fileStream = new FileStream(saveFileDialogPS.FileName, FileMode.Create);
			StreamWriter file = new StreamWriter(fileStream);
			file.WriteLine("Options = {}");
			file.Write("Options.Particles = ");
			file.WriteLine(numericUpDownParticles.Value);
			file.Write("Options.Coordinates = ");
			file.WriteLine(numericUpDownCoordinates.Value);
			file.Write("Options.Colors = ");
			file.WriteLine(numericUpDownColors.Value);
			file.Write("Options.Sizes = ");
			file.WriteLine(numericUpDownSizes.Value);
			file.Write("Options.Texture = \"");
			int i;
			for (i = 0; i < saveFileDialogPS.FileName.Length && i < openFileDialogTex.FileName.Length; i++)
				if (saveFileDialogPS.FileName[i] != openFileDialogTex.FileName[i])
					break;
			for (int j = i; j < saveFileDialogPS.FileName.Length; j++)
				if (saveFileDialogPS.FileName[j] == '\\')
					file.Write("../");
			file.WriteLine(openFileDialogTex.FileName.Substring(i, openFileDialogTex.FileName.Length - i).Replace('\\', '/') + "\"");
			file.Write("Options.Looping = ");
			file.WriteLine(checkBoxLooping.Checked.ToString().ToLower());
			file.WriteLine("initParticleSystem(Options)");
			file.Write(textBox1.Text);
			file.Close();
			fileStream.Close();
			if (sender != buttonReload)
			{
				statusBar1.Text = "Particle system saved";
				Text = "Particle Editor - " + Path.GetFileName(saveFileDialogPS.FileName);
				Dirty = false;
			}
			return true;
		}

		private void menuItemOpen_Click(object sender, System.EventArgs e)
		{
			if (!discardChanges())
				return;
			if (openFileDialogPS.ShowDialog() != DialogResult.OK)
				return;
			char[] temp = new char[32];
			FileStream fileStream = new FileStream(openFileDialogPS.FileName, FileMode.Open);
			StreamReader file = new StreamReader(fileStream);
			file.ReadLine();
			file.Read(temp, 0, "Options.Particles = ".Length);
			numericUpDownParticles.Value = Convert.ToInt32(file.ReadLine());
			file.Read(temp, 0, "Options.Coordinates = ".Length);
			numericUpDownCoordinates.Value = Convert.ToInt32(file.ReadLine());
			file.Read(temp, 0, "Options.Colors = ".Length);
			numericUpDownColors.Value = Convert.ToInt32(file.ReadLine());
			file.Read(temp, 0, "Options.Sizes = ".Length);
			numericUpDownSizes.Value = Convert.ToInt32(file.ReadLine());
			file.Read(temp, 0, "Options.Texture = ".Length);
			String dir = Path.GetDirectoryName(openFileDialogPS.FileName);
			String texture = file.ReadLine();
			openFileDialogTex.FileName = Path.GetFullPath(dir + '/' + texture.Substring(1, texture.Length - 2));
			file.Read(temp, 0, "Options.Looping = ".Length);
			checkBoxLooping.Checked = Convert.ToBoolean(file.ReadLine());
			file.ReadLine();
			textBox1.Text = file.ReadToEnd();
			file.Close();
			fileStream.Close();
			statusBar1.Text = "Particle system loaded";
			saveFileDialogPS.FileName = openFileDialogPS.FileName;
			Text = "Particle Editor - " + Path.GetFileName(saveFileDialogPS.FileName);
			Dirty = false;
		}

		private void valueChanged(object sender, System.EventArgs e)
		{
			Dirty = true;
		}

		private void menuItemNew_Click(object sender, System.EventArgs e)
		{
			if (sender != null)
				if (!discardChanges())
					return;
			numericUpDownParticles.Value = 100;
			numericUpDownCoordinates.Value = 2;
			numericUpDownColors.Value = 1;
			numericUpDownSizes.Value = 1;
			checkBoxLooping.Checked = true;
			openFileDialogTex.FileName = Path.GetFullPath(homeDir + "/../../particles/32x32_default.bmp");
			pictureBoxTexture.Image = Image.FromFile(openFileDialogTex.FileName);
//			particleSystem = new ParticleLib.ParticleSystem(100, 2, 1, 1, openFileDialogTex.FileName);
			textBox1.Text = "";
			xRot = yRot = 0;
			dist = 20;
			Dirty = false;
		}
		
		private bool discardChanges()
		{
			if (!Dirty)
				return true;
			String file = saveFileDialogPS.FileName != "" ? Path.GetFileName(saveFileDialogPS.FileName) : "Untitled";
			DialogResult result = MessageBox.Show("Save changes to " + file + "?", "Save changes?", MessageBoxButtons.YesNoCancel, MessageBoxIcon.Exclamation);
			if (result == DialogResult.Yes)
				return save(menuItemSave);
			if (result == DialogResult.Cancel)
				return false;
			if (result == DialogResult.No)
				return true;
			return false;
		}

		private void Form1_Closing(object sender, System.ComponentModel.CancelEventArgs e)
		{
			if (!discardChanges())
				e.Cancel = true;
		}

		private void menuItemExit_Click(object sender, System.EventArgs e)
		{
			this.Close();
		}

		private void buttonReload_Click(object sender, System.EventArgs e)
		{
			String temp = saveFileDialogPS.FileName;
			saveFileDialogPS.FileName = Path.GetFullPath("ParticleEditorTemp.dat");
			save(buttonReload);
			try 
			{
				particleSystem = new ParticleLib.ParticleSystem(saveFileDialogPS.FileName.Replace('\\', '/'));
				statusBar1.Text = "Ready";
			}
			catch (ApplicationException ae)
			{
				particleSystem = null;
				String error = ae.Message.Substring(ae.Message.IndexOf(':') + 1);
				int line = Convert.ToInt32(error.Substring(0, error.IndexOf(':')));
				statusBar1.Text = "Line " + (line - 8) + error.Substring(error.IndexOf(':'));
			}
			saveFileDialogPS.FileName = temp;
			time = 0;
		}

		private void buttonPlay_Click(object sender, System.EventArgs e)
		{
			if (play)
			{
				buttonPlay.ImageIndex = 6;
				play = false;
			}
			else
			{
				buttonPlay.ImageIndex = 7;
				play = true;
			}
		}
	}
}
