/*
 * VTFEdit
 * Copyright (C) 2005-2010 Neil Jedrzejewski & Ryan Gregg
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#pragma once

#include "VMTFileUtility.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

namespace VTFEdit
{
	public ref class CVMTCreate : public System::Windows::Forms::Form
	{
	public: 
		CVMTCreate(void)
		{
			this->InitializeComponent();
		}

	private: System::Windows::Forms::OpenFileDialog ^  dlgOpenVTFFile;
	private: System::Windows::Forms::GroupBox ^  grpTextures;
	private: System::Windows::Forms::Button ^  btnBaseTexture1;
	private: System::Windows::Forms::TextBox ^  txtBaseTexture1;
	private: System::Windows::Forms::Label ^  lblBaseTexture1;
	private: System::Windows::Forms::TabControl ^  tabMain;
	private: System::Windows::Forms::TabPage ^  tabTextures;
	private: System::Windows::Forms::Button ^  btnBumpTexture1;
	private: System::Windows::Forms::TextBox ^  txtBumpTexture1;
	private: System::Windows::Forms::Label ^  lblBumpTexture1;
	private: System::Windows::Forms::Button ^  btnEnviromentTexture;
	private: System::Windows::Forms::TextBox ^  txtEnviromentTexture;
	private: System::Windows::Forms::Label ^  lblEnviromentTexture;
	private: System::Windows::Forms::Button ^  btnEnviromentMaskTexture;
	private: System::Windows::Forms::TextBox ^  txtEnviromentMaskTexture;
	private: System::Windows::Forms::Label ^  lblEnviromentMaskTexture;
	private: System::Windows::Forms::Button ^  btnDetailTexture;
	private: System::Windows::Forms::TextBox ^  txtDetailTexture;
	private: System::Windows::Forms::Label ^  lblDetailTexture;
	private: System::Windows::Forms::ToolTip ^  tipMain;
	private: System::Windows::Forms::Button ^  btnClose;
	private: System::Windows::Forms::Button ^  btnCreate;
	private: System::Windows::Forms::Button ^  btnToolTexture;
	private: System::Windows::Forms::TextBox ^  txtToolTexture;
	private: System::Windows::Forms::Label ^  lblToolTexture;
	private: System::Windows::Forms::Button ^  btnBumpTexture2;
	private: System::Windows::Forms::TextBox ^  txtBumpTexture2;
	private: System::Windows::Forms::Label ^  lblBumpTexture2;
	private: System::Windows::Forms::Label ^  lblBaseTexture2;
	private: System::Windows::Forms::Button ^  btnNormalTexture;
	private: System::Windows::Forms::TextBox ^  txtNormalTexture;
	private: System::Windows::Forms::Label ^  lblNormalTexture;
	private: System::Windows::Forms::Button ^  btnDuDvTexture;
	private: System::Windows::Forms::TextBox ^  txtDuDvTexture;
	private: System::Windows::Forms::Label ^  lblDuDvTexture;
	private: System::Windows::Forms::Button ^  btnBaseTexture2;
	private: System::Windows::Forms::TextBox ^  txtBaseTexture2;
	private: System::Windows::Forms::TabPage ^  tabOptions;
	private: System::Windows::Forms::GroupBox ^  grpOptions;
	private: System::Windows::Forms::Button ^  btnClear;
	private: System::Windows::Forms::SaveFileDialog ^  dlgSaveVMTFile;
	private: System::Windows::Forms::Label ^  lblShader;
	private: System::Windows::Forms::ComboBox ^  cboShader;
	private: System::Windows::Forms::ComboBox ^  cboSurface1;
	private: System::Windows::Forms::Label ^  lblSurface1;
	private: System::Windows::Forms::ComboBox ^  cboSurface2;
	private: System::Windows::Forms::Label ^  lblSurface2;
	private: System::Windows::Forms::Label ^  lblKeywords;
	private: System::Windows::Forms::TextBox ^  txtKeywords;
	private: System::Windows::Forms::CheckBox ^  chkTranslucent;
	private: System::Windows::Forms::CheckBox ^  chkAdditive;
	private: System::Windows::Forms::CheckBox ^  chkEnviromentMapContrast;
	private: System::Windows::Forms::CheckBox ^  chkEnviromentMapSaturation;
	private: System::Windows::Forms::CheckBox ^  chkNoCull;
	private: System::Windows::Forms::CheckBox ^  chkAlphaTest;
	private: System::Windows::Forms::CheckBox ^  chkNoLOD;
	private: System::Windows::Forms::CheckBox ^  chkNoDecal;
	private: System::Windows::Forms::CheckBox ^  chkVertexColor;
	private: System::Windows::Forms::CheckBox ^  chkVertexAlpha;
	private: System::ComponentModel::IContainer ^  components;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(CVMTCreate::typeid));
			this->grpTextures = (gcnew System::Windows::Forms::GroupBox());
			this->btnNormalTexture = (gcnew System::Windows::Forms::Button());
			this->txtNormalTexture = (gcnew System::Windows::Forms::TextBox());
			this->lblNormalTexture = (gcnew System::Windows::Forms::Label());
			this->btnDuDvTexture = (gcnew System::Windows::Forms::Button());
			this->txtDuDvTexture = (gcnew System::Windows::Forms::TextBox());
			this->lblDuDvTexture = (gcnew System::Windows::Forms::Label());
			this->btnBumpTexture2 = (gcnew System::Windows::Forms::Button());
			this->txtBumpTexture2 = (gcnew System::Windows::Forms::TextBox());
			this->lblBumpTexture2 = (gcnew System::Windows::Forms::Label());
			this->btnBaseTexture2 = (gcnew System::Windows::Forms::Button());
			this->txtBaseTexture2 = (gcnew System::Windows::Forms::TextBox());
			this->lblBaseTexture2 = (gcnew System::Windows::Forms::Label());
			this->btnToolTexture = (gcnew System::Windows::Forms::Button());
			this->txtToolTexture = (gcnew System::Windows::Forms::TextBox());
			this->lblToolTexture = (gcnew System::Windows::Forms::Label());
			this->btnDetailTexture = (gcnew System::Windows::Forms::Button());
			this->txtDetailTexture = (gcnew System::Windows::Forms::TextBox());
			this->lblDetailTexture = (gcnew System::Windows::Forms::Label());
			this->btnEnviromentMaskTexture = (gcnew System::Windows::Forms::Button());
			this->txtEnviromentMaskTexture = (gcnew System::Windows::Forms::TextBox());
			this->lblEnviromentMaskTexture = (gcnew System::Windows::Forms::Label());
			this->btnEnviromentTexture = (gcnew System::Windows::Forms::Button());
			this->txtEnviromentTexture = (gcnew System::Windows::Forms::TextBox());
			this->lblEnviromentTexture = (gcnew System::Windows::Forms::Label());
			this->btnBumpTexture1 = (gcnew System::Windows::Forms::Button());
			this->txtBumpTexture1 = (gcnew System::Windows::Forms::TextBox());
			this->lblBumpTexture1 = (gcnew System::Windows::Forms::Label());
			this->btnBaseTexture1 = (gcnew System::Windows::Forms::Button());
			this->txtBaseTexture1 = (gcnew System::Windows::Forms::TextBox());
			this->lblBaseTexture1 = (gcnew System::Windows::Forms::Label());
			this->dlgOpenVTFFile = (gcnew System::Windows::Forms::OpenFileDialog());
			this->tabMain = (gcnew System::Windows::Forms::TabControl());
			this->tabTextures = (gcnew System::Windows::Forms::TabPage());
			this->tabOptions = (gcnew System::Windows::Forms::TabPage());
			this->grpOptions = (gcnew System::Windows::Forms::GroupBox());
			this->chkVertexAlpha = (gcnew System::Windows::Forms::CheckBox());
			this->chkVertexColor = (gcnew System::Windows::Forms::CheckBox());
			this->chkNoDecal = (gcnew System::Windows::Forms::CheckBox());
			this->chkNoLOD = (gcnew System::Windows::Forms::CheckBox());
			this->chkAlphaTest = (gcnew System::Windows::Forms::CheckBox());
			this->chkNoCull = (gcnew System::Windows::Forms::CheckBox());
			this->chkEnviromentMapSaturation = (gcnew System::Windows::Forms::CheckBox());
			this->chkEnviromentMapContrast = (gcnew System::Windows::Forms::CheckBox());
			this->chkAdditive = (gcnew System::Windows::Forms::CheckBox());
			this->chkTranslucent = (gcnew System::Windows::Forms::CheckBox());
			this->txtKeywords = (gcnew System::Windows::Forms::TextBox());
			this->lblKeywords = (gcnew System::Windows::Forms::Label());
			this->cboSurface2 = (gcnew System::Windows::Forms::ComboBox());
			this->lblSurface2 = (gcnew System::Windows::Forms::Label());
			this->cboSurface1 = (gcnew System::Windows::Forms::ComboBox());
			this->lblSurface1 = (gcnew System::Windows::Forms::Label());
			this->cboShader = (gcnew System::Windows::Forms::ComboBox());
			this->lblShader = (gcnew System::Windows::Forms::Label());
			this->tipMain = (gcnew System::Windows::Forms::ToolTip(this->components));
			this->btnClose = (gcnew System::Windows::Forms::Button());
			this->btnCreate = (gcnew System::Windows::Forms::Button());
			this->btnClear = (gcnew System::Windows::Forms::Button());
			this->dlgSaveVMTFile = (gcnew System::Windows::Forms::SaveFileDialog());
			this->grpTextures->SuspendLayout();
			this->tabMain->SuspendLayout();
			this->tabTextures->SuspendLayout();
			this->tabOptions->SuspendLayout();
			this->grpOptions->SuspendLayout();
			this->SuspendLayout();
			// 
			// grpTextures
			// 
			this->grpTextures->Controls->Add(this->btnNormalTexture);
			this->grpTextures->Controls->Add(this->txtNormalTexture);
			this->grpTextures->Controls->Add(this->lblNormalTexture);
			this->grpTextures->Controls->Add(this->btnDuDvTexture);
			this->grpTextures->Controls->Add(this->txtDuDvTexture);
			this->grpTextures->Controls->Add(this->lblDuDvTexture);
			this->grpTextures->Controls->Add(this->btnBumpTexture2);
			this->grpTextures->Controls->Add(this->txtBumpTexture2);
			this->grpTextures->Controls->Add(this->lblBumpTexture2);
			this->grpTextures->Controls->Add(this->btnBaseTexture2);
			this->grpTextures->Controls->Add(this->txtBaseTexture2);
			this->grpTextures->Controls->Add(this->lblBaseTexture2);
			this->grpTextures->Controls->Add(this->btnToolTexture);
			this->grpTextures->Controls->Add(this->txtToolTexture);
			this->grpTextures->Controls->Add(this->lblToolTexture);
			this->grpTextures->Controls->Add(this->btnDetailTexture);
			this->grpTextures->Controls->Add(this->txtDetailTexture);
			this->grpTextures->Controls->Add(this->lblDetailTexture);
			this->grpTextures->Controls->Add(this->btnEnviromentMaskTexture);
			this->grpTextures->Controls->Add(this->txtEnviromentMaskTexture);
			this->grpTextures->Controls->Add(this->lblEnviromentMaskTexture);
			this->grpTextures->Controls->Add(this->btnEnviromentTexture);
			this->grpTextures->Controls->Add(this->txtEnviromentTexture);
			this->grpTextures->Controls->Add(this->lblEnviromentTexture);
			this->grpTextures->Controls->Add(this->btnBumpTexture1);
			this->grpTextures->Controls->Add(this->txtBumpTexture1);
			this->grpTextures->Controls->Add(this->lblBumpTexture1);
			this->grpTextures->Controls->Add(this->btnBaseTexture1);
			this->grpTextures->Controls->Add(this->txtBaseTexture1);
			this->grpTextures->Controls->Add(this->lblBaseTexture1);
			this->grpTextures->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->grpTextures->Location = System::Drawing::Point(10, 9);
			this->grpTextures->Name = L"grpTextures";
			this->grpTextures->Size = System::Drawing::Size(497, 305);
			this->grpTextures->TabIndex = 0;
			this->grpTextures->TabStop = false;
			this->grpTextures->Text = L"Textures:";
			// 
			// btnNormalTexture
			// 
			this->btnNormalTexture->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->btnNormalTexture->Location = System::Drawing::Point(467, 240);
			this->btnNormalTexture->Name = L"btnNormalTexture";
			this->btnNormalTexture->Size = System::Drawing::Size(24, 23);
			this->btnNormalTexture->TabIndex = 26;
			this->btnNormalTexture->Text = L"...";
			this->btnNormalTexture->Click += gcnew System::EventHandler(this, &CVMTCreate::btnNormalTexture_Click);
			// 
			// txtNormalTexture
			// 
			this->txtNormalTexture->Location = System::Drawing::Point(149, 240);
			this->txtNormalTexture->Name = L"txtNormalTexture";
			this->txtNormalTexture->Size = System::Drawing::Size(312, 26);
			this->txtNormalTexture->TabIndex = 25;
			this->tipMain->SetToolTip(this->txtNormalTexture, L"$normalmap");
			this->txtNormalTexture->Leave += gcnew System::EventHandler(this, &CVMTCreate::txtTexture_Leave);
			// 
			// lblNormalTexture
			// 
			this->lblNormalTexture->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->lblNormalTexture->Location = System::Drawing::Point(10, 240);
			this->lblNormalTexture->Name = L"lblNormalTexture";
			this->lblNormalTexture->Size = System::Drawing::Size(133, 27);
			this->lblNormalTexture->TabIndex = 24;
			this->lblNormalTexture->Text = L"Normal Map:";
			// 
			// btnDuDvTexture
			// 
			this->btnDuDvTexture->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->btnDuDvTexture->Location = System::Drawing::Point(467, 268);
			this->btnDuDvTexture->Name = L"btnDuDvTexture";
			this->btnDuDvTexture->Size = System::Drawing::Size(24, 23);
			this->btnDuDvTexture->TabIndex = 29;
			this->btnDuDvTexture->Text = L"...";
			this->btnDuDvTexture->Click += gcnew System::EventHandler(this, &CVMTCreate::btnDuDvTexture_Click);
			// 
			// txtDuDvTexture
			// 
			this->txtDuDvTexture->Location = System::Drawing::Point(149, 268);
			this->txtDuDvTexture->Name = L"txtDuDvTexture";
			this->txtDuDvTexture->Size = System::Drawing::Size(312, 26);
			this->txtDuDvTexture->TabIndex = 28;
			this->tipMain->SetToolTip(this->txtDuDvTexture, L"$dudvmap");
			this->txtDuDvTexture->Leave += gcnew System::EventHandler(this, &CVMTCreate::txtTexture_Leave);
			// 
			// lblDuDvTexture
			// 
			this->lblDuDvTexture->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->lblDuDvTexture->Location = System::Drawing::Point(10, 268);
			this->lblDuDvTexture->Name = L"lblDuDvTexture";
			this->lblDuDvTexture->Size = System::Drawing::Size(133, 26);
			this->lblDuDvTexture->TabIndex = 27;
			this->lblDuDvTexture->Text = L"DuDv Map:";
			// 
			// btnBumpTexture2
			// 
			this->btnBumpTexture2->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->btnBumpTexture2->Location = System::Drawing::Point(467, 102);
			this->btnBumpTexture2->Name = L"btnBumpTexture2";
			this->btnBumpTexture2->Size = System::Drawing::Size(24, 23);
			this->btnBumpTexture2->TabIndex = 11;
			this->btnBumpTexture2->Text = L"...";
			this->btnBumpTexture2->Click += gcnew System::EventHandler(this, &CVMTCreate::btnBumpTexture2_Click);
			// 
			// txtBumpTexture2
			// 
			this->txtBumpTexture2->Location = System::Drawing::Point(149, 102);
			this->txtBumpTexture2->Name = L"txtBumpTexture2";
			this->txtBumpTexture2->Size = System::Drawing::Size(312, 26);
			this->txtBumpTexture2->TabIndex = 10;
			this->tipMain->SetToolTip(this->txtBumpTexture2, L"$bumpmap2");
			this->txtBumpTexture2->Leave += gcnew System::EventHandler(this, &CVMTCreate::txtTexture_Leave);
			// 
			// lblBumpTexture2
			// 
			this->lblBumpTexture2->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->lblBumpTexture2->Location = System::Drawing::Point(10, 102);
			this->lblBumpTexture2->Name = L"lblBumpTexture2";
			this->lblBumpTexture2->Size = System::Drawing::Size(133, 26);
			this->lblBumpTexture2->TabIndex = 9;
			this->lblBumpTexture2->Text = L"Bump Map 2:";
			// 
			// btnBaseTexture2
			// 
			this->btnBaseTexture2->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->btnBaseTexture2->Location = System::Drawing::Point(467, 74);
			this->btnBaseTexture2->Name = L"btnBaseTexture2";
			this->btnBaseTexture2->Size = System::Drawing::Size(24, 23);
			this->btnBaseTexture2->TabIndex = 8;
			this->btnBaseTexture2->Text = L"...";
			this->btnBaseTexture2->Click += gcnew System::EventHandler(this, &CVMTCreate::btnBaseTexture2_Click);
			// 
			// txtBaseTexture2
			// 
			this->txtBaseTexture2->Location = System::Drawing::Point(149, 74);
			this->txtBaseTexture2->Name = L"txtBaseTexture2";
			this->txtBaseTexture2->Size = System::Drawing::Size(312, 26);
			this->txtBaseTexture2->TabIndex = 7;
			this->tipMain->SetToolTip(this->txtBaseTexture2, L"$basetexture2");
			this->txtBaseTexture2->Leave += gcnew System::EventHandler(this, &CVMTCreate::txtTexture_Leave);
			// 
			// lblBaseTexture2
			// 
			this->lblBaseTexture2->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->lblBaseTexture2->Location = System::Drawing::Point(10, 74);
			this->lblBaseTexture2->Name = L"lblBaseTexture2";
			this->lblBaseTexture2->Size = System::Drawing::Size(133, 26);
			this->lblBaseTexture2->TabIndex = 6;
			this->lblBaseTexture2->Text = L"Base Texture 2:";
			// 
			// btnToolTexture
			// 
			this->btnToolTexture->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->btnToolTexture->Location = System::Drawing::Point(467, 212);
			this->btnToolTexture->Name = L"btnToolTexture";
			this->btnToolTexture->Size = System::Drawing::Size(24, 23);
			this->btnToolTexture->TabIndex = 23;
			this->btnToolTexture->Text = L"...";
			this->btnToolTexture->Click += gcnew System::EventHandler(this, &CVMTCreate::btnToolTexture_Click);
			// 
			// txtToolTexture
			// 
			this->txtToolTexture->Location = System::Drawing::Point(149, 212);
			this->txtToolTexture->Name = L"txtToolTexture";
			this->txtToolTexture->Size = System::Drawing::Size(312, 26);
			this->txtToolTexture->TabIndex = 22;
			this->tipMain->SetToolTip(this->txtToolTexture, L"%tooltexture");
			this->txtToolTexture->Leave += gcnew System::EventHandler(this, &CVMTCreate::txtTexture_Leave);
			// 
			// lblToolTexture
			// 
			this->lblToolTexture->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->lblToolTexture->Location = System::Drawing::Point(10, 212);
			this->lblToolTexture->Name = L"lblToolTexture";
			this->lblToolTexture->Size = System::Drawing::Size(133, 27);
			this->lblToolTexture->TabIndex = 21;
			this->lblToolTexture->Text = L"Tool Texture:";
			// 
			// btnDetailTexture
			// 
			this->btnDetailTexture->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->btnDetailTexture->Location = System::Drawing::Point(467, 185);
			this->btnDetailTexture->Name = L"btnDetailTexture";
			this->btnDetailTexture->Size = System::Drawing::Size(24, 23);
			this->btnDetailTexture->TabIndex = 20;
			this->btnDetailTexture->Text = L"...";
			this->btnDetailTexture->Click += gcnew System::EventHandler(this, &CVMTCreate::btnDetailTexture_Click);
			// 
			// txtDetailTexture
			// 
			this->txtDetailTexture->Location = System::Drawing::Point(149, 185);
			this->txtDetailTexture->Name = L"txtDetailTexture";
			this->txtDetailTexture->Size = System::Drawing::Size(312, 26);
			this->txtDetailTexture->TabIndex = 19;
			this->tipMain->SetToolTip(this->txtDetailTexture, L"$detail");
			this->txtDetailTexture->Leave += gcnew System::EventHandler(this, &CVMTCreate::txtTexture_Leave);
			// 
			// lblDetailTexture
			// 
			this->lblDetailTexture->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->lblDetailTexture->Location = System::Drawing::Point(10, 185);
			this->lblDetailTexture->Name = L"lblDetailTexture";
			this->lblDetailTexture->Size = System::Drawing::Size(133, 26);
			this->lblDetailTexture->TabIndex = 18;
			this->lblDetailTexture->Text = L"Detail Texture:";
			// 
			// btnEnviromentMaskTexture
			// 
			this->btnEnviromentMaskTexture->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->btnEnviromentMaskTexture->Location = System::Drawing::Point(467, 157);
			this->btnEnviromentMaskTexture->Name = L"btnEnviromentMaskTexture";
			this->btnEnviromentMaskTexture->Size = System::Drawing::Size(24, 23);
			this->btnEnviromentMaskTexture->TabIndex = 17;
			this->btnEnviromentMaskTexture->Text = L"...";
			this->btnEnviromentMaskTexture->Click += gcnew System::EventHandler(this, &CVMTCreate::btnEnviromentMaskTexture_Click);
			// 
			// txtEnviromentMaskTexture
			// 
			this->txtEnviromentMaskTexture->Location = System::Drawing::Point(149, 157);
			this->txtEnviromentMaskTexture->Name = L"txtEnviromentMaskTexture";
			this->txtEnviromentMaskTexture->Size = System::Drawing::Size(312, 26);
			this->txtEnviromentMaskTexture->TabIndex = 16;
			this->tipMain->SetToolTip(this->txtEnviromentMaskTexture, L"$envmapmask");
			this->txtEnviromentMaskTexture->Leave += gcnew System::EventHandler(this, &CVMTCreate::txtTexture_Leave);
			// 
			// lblEnviromentMaskTexture
			// 
			this->lblEnviromentMaskTexture->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->lblEnviromentMaskTexture->Location = System::Drawing::Point(10, 157);
			this->lblEnviromentMaskTexture->Name = L"lblEnviromentMaskTexture";
			this->lblEnviromentMaskTexture->Size = System::Drawing::Size(133, 26);
			this->lblEnviromentMaskTexture->TabIndex = 15;
			this->lblEnviromentMaskTexture->Text = L"Enviroment Mask:";
			// 
			// btnEnviromentTexture
			// 
			this->btnEnviromentTexture->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->btnEnviromentTexture->Location = System::Drawing::Point(467, 129);
			this->btnEnviromentTexture->Name = L"btnEnviromentTexture";
			this->btnEnviromentTexture->Size = System::Drawing::Size(24, 23);
			this->btnEnviromentTexture->TabIndex = 14;
			this->btnEnviromentTexture->Text = L"...";
			this->btnEnviromentTexture->Click += gcnew System::EventHandler(this, &CVMTCreate::btnEnviromentTexture_Click);
			// 
			// txtEnviromentTexture
			// 
			this->txtEnviromentTexture->Location = System::Drawing::Point(149, 129);
			this->txtEnviromentTexture->Name = L"txtEnviromentTexture";
			this->txtEnviromentTexture->Size = System::Drawing::Size(312, 26);
			this->txtEnviromentTexture->TabIndex = 13;
			this->tipMain->SetToolTip(this->txtEnviromentTexture, L"$envmap");
			this->txtEnviromentTexture->Leave += gcnew System::EventHandler(this, &CVMTCreate::txtTexture_Leave);
			// 
			// lblEnviromentTexture
			// 
			this->lblEnviromentTexture->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->lblEnviromentTexture->Location = System::Drawing::Point(10, 129);
			this->lblEnviromentTexture->Name = L"lblEnviromentTexture";
			this->lblEnviromentTexture->Size = System::Drawing::Size(133, 27);
			this->lblEnviromentTexture->TabIndex = 12;
			this->lblEnviromentTexture->Text = L"Enviroment:";
			// 
			// btnBumpTexture1
			// 
			this->btnBumpTexture1->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->btnBumpTexture1->Location = System::Drawing::Point(467, 46);
			this->btnBumpTexture1->Name = L"btnBumpTexture1";
			this->btnBumpTexture1->Size = System::Drawing::Size(24, 23);
			this->btnBumpTexture1->TabIndex = 5;
			this->btnBumpTexture1->Text = L"...";
			this->btnBumpTexture1->Click += gcnew System::EventHandler(this, &CVMTCreate::btnBumpTexture1_Click);
			// 
			// txtBumpTexture1
			// 
			this->txtBumpTexture1->Location = System::Drawing::Point(149, 46);
			this->txtBumpTexture1->Name = L"txtBumpTexture1";
			this->txtBumpTexture1->Size = System::Drawing::Size(312, 26);
			this->txtBumpTexture1->TabIndex = 4;
			this->tipMain->SetToolTip(this->txtBumpTexture1, L"$bumpmap");
			this->txtBumpTexture1->Leave += gcnew System::EventHandler(this, &CVMTCreate::txtTexture_Leave);
			// 
			// lblBumpTexture1
			// 
			this->lblBumpTexture1->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->lblBumpTexture1->Location = System::Drawing::Point(10, 46);
			this->lblBumpTexture1->Name = L"lblBumpTexture1";
			this->lblBumpTexture1->Size = System::Drawing::Size(133, 27);
			this->lblBumpTexture1->TabIndex = 3;
			this->lblBumpTexture1->Text = L"Bump Map 1:";
			// 
			// btnBaseTexture1
			// 
			this->btnBaseTexture1->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->btnBaseTexture1->Location = System::Drawing::Point(467, 18);
			this->btnBaseTexture1->Name = L"btnBaseTexture1";
			this->btnBaseTexture1->Size = System::Drawing::Size(24, 24);
			this->btnBaseTexture1->TabIndex = 2;
			this->btnBaseTexture1->Text = L"...";
			this->btnBaseTexture1->Click += gcnew System::EventHandler(this, &CVMTCreate::btnBaseTexture1_Click);
			// 
			// txtBaseTexture1
			// 
			this->txtBaseTexture1->Location = System::Drawing::Point(149, 18);
			this->txtBaseTexture1->Name = L"txtBaseTexture1";
			this->txtBaseTexture1->Size = System::Drawing::Size(312, 26);
			this->txtBaseTexture1->TabIndex = 1;
			this->tipMain->SetToolTip(this->txtBaseTexture1, L"$basetexture");
			this->txtBaseTexture1->TextChanged += gcnew System::EventHandler(this, &CVMTCreate::txtBaseTexture1_TextChanged);
			this->txtBaseTexture1->Leave += gcnew System::EventHandler(this, &CVMTCreate::txtTexture_Leave);
			// 
			// lblBaseTexture1
			// 
			this->lblBaseTexture1->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->lblBaseTexture1->Location = System::Drawing::Point(10, 18);
			this->lblBaseTexture1->Name = L"lblBaseTexture1";
			this->lblBaseTexture1->Size = System::Drawing::Size(133, 27);
			this->lblBaseTexture1->TabIndex = 0;
			this->lblBaseTexture1->Text = L"Base Texture 1:";
			// 
			// dlgOpenVTFFile
			// 
			this->dlgOpenVTFFile->Filter = L"VTF Files (*.vtf)|*.vtf";
			this->dlgOpenVTFFile->Title = L"Open VTF File";
			// 
			// tabMain
			// 
			this->tabMain->Controls->Add(this->tabTextures);
			this->tabMain->Controls->Add(this->tabOptions);
			this->tabMain->Location = System::Drawing::Point(10, 9);
			this->tabMain->Name = L"tabMain";
			this->tabMain->SelectedIndex = 0;
			this->tabMain->Size = System::Drawing::Size(518, 351);
			this->tabMain->TabIndex = 0;
			// 
			// tabTextures
			// 
			this->tabTextures->Controls->Add(this->grpTextures);
			this->tabTextures->Location = System::Drawing::Point(4, 29);
			this->tabTextures->Name = L"tabTextures";
			this->tabTextures->Size = System::Drawing::Size(510, 318);
			this->tabTextures->TabIndex = 0;
			this->tabTextures->Text = L"Textures";
			// 
			// tabOptions
			// 
			this->tabOptions->Controls->Add(this->grpOptions);
			this->tabOptions->Location = System::Drawing::Point(4, 29);
			this->tabOptions->Name = L"tabOptions";
			this->tabOptions->Size = System::Drawing::Size(510, 318);
			this->tabOptions->TabIndex = 1;
			this->tabOptions->Text = L"Options";
			// 
			// grpOptions
			// 
			this->grpOptions->Controls->Add(this->chkVertexAlpha);
			this->grpOptions->Controls->Add(this->chkVertexColor);
			this->grpOptions->Controls->Add(this->chkNoDecal);
			this->grpOptions->Controls->Add(this->chkNoLOD);
			this->grpOptions->Controls->Add(this->chkAlphaTest);
			this->grpOptions->Controls->Add(this->chkNoCull);
			this->grpOptions->Controls->Add(this->chkEnviromentMapSaturation);
			this->grpOptions->Controls->Add(this->chkEnviromentMapContrast);
			this->grpOptions->Controls->Add(this->chkAdditive);
			this->grpOptions->Controls->Add(this->chkTranslucent);
			this->grpOptions->Controls->Add(this->txtKeywords);
			this->grpOptions->Controls->Add(this->lblKeywords);
			this->grpOptions->Controls->Add(this->cboSurface2);
			this->grpOptions->Controls->Add(this->lblSurface2);
			this->grpOptions->Controls->Add(this->cboSurface1);
			this->grpOptions->Controls->Add(this->lblSurface1);
			this->grpOptions->Controls->Add(this->cboShader);
			this->grpOptions->Controls->Add(this->lblShader);
			this->grpOptions->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->grpOptions->Location = System::Drawing::Point(10, 9);
			this->grpOptions->Name = L"grpOptions";
			this->grpOptions->Size = System::Drawing::Size(481, 305);
			this->grpOptions->TabIndex = 0;
			this->grpOptions->TabStop = false;
			this->grpOptions->Text = L"Options:";
			this->grpOptions->Enter += gcnew System::EventHandler(this, &CVMTCreate::grpOptions_Enter);
			// 
			// chkVertexAlpha
			// 
			this->chkVertexAlpha->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->chkVertexAlpha->Location = System::Drawing::Point(270, 185);
			this->chkVertexAlpha->Name = L"chkVertexAlpha";
			this->chkVertexAlpha->Size = System::Drawing::Size(192, 27);
			this->chkVertexAlpha->TabIndex = 16;
			this->chkVertexAlpha->Text = L"Vertex Alpha";
			this->tipMain->SetToolTip(this->chkVertexAlpha, L"$vertexalpha");
			// 
			// chkVertexColor
			// 
			this->chkVertexColor->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->chkVertexColor->Location = System::Drawing::Point(270, 212);
			this->chkVertexColor->Name = L"chkVertexColor";
			this->chkVertexColor->Size = System::Drawing::Size(192, 28);
			this->chkVertexColor->TabIndex = 17;
			this->chkVertexColor->Text = L"Vertex Color";
			this->tipMain->SetToolTip(this->chkVertexColor, L"$vertexcolor");
			// 
			// chkNoDecal
			// 
			this->chkNoDecal->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->chkNoDecal->Location = System::Drawing::Point(10, 268);
			this->chkNoDecal->Name = L"chkNoDecal";
			this->chkNoDecal->Size = System::Drawing::Size(192, 27);
			this->chkNoDecal->TabIndex = 13;
			this->chkNoDecal->Text = L"No Decal";
			this->tipMain->SetToolTip(this->chkNoDecal, L"$nodecal");
			// 
			// chkNoLOD
			// 
			this->chkNoLOD->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->chkNoLOD->Location = System::Drawing::Point(270, 129);
			this->chkNoLOD->Name = L"chkNoLOD";
			this->chkNoLOD->Size = System::Drawing::Size(192, 28);
			this->chkNoLOD->TabIndex = 14;
			this->chkNoLOD->Text = L"No LOD";
			this->tipMain->SetToolTip(this->chkNoLOD, L"$nolod");
			// 
			// chkAlphaTest
			// 
			this->chkAlphaTest->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->chkAlphaTest->Location = System::Drawing::Point(10, 157);
			this->chkAlphaTest->Name = L"chkAlphaTest";
			this->chkAlphaTest->Size = System::Drawing::Size(192, 28);
			this->chkAlphaTest->TabIndex = 9;
			this->chkAlphaTest->Text = L"Alpha Test";
			this->tipMain->SetToolTip(this->chkAlphaTest, L"$alphatest");
			// 
			// chkNoCull
			// 
			this->chkNoCull->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->chkNoCull->Location = System::Drawing::Point(10, 240);
			this->chkNoCull->Name = L"chkNoCull";
			this->chkNoCull->Size = System::Drawing::Size(192, 28);
			this->chkNoCull->TabIndex = 12;
			this->chkNoCull->Text = L"No Cull";
			this->tipMain->SetToolTip(this->chkNoCull, L"$nocull");
			// 
			// chkEnviromentMapSaturation
			// 
			this->chkEnviromentMapSaturation->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->chkEnviromentMapSaturation->Location = System::Drawing::Point(10, 212);
			this->chkEnviromentMapSaturation->Name = L"chkEnviromentMapSaturation";
			this->chkEnviromentMapSaturation->Size = System::Drawing::Size(234, 28);
			this->chkEnviromentMapSaturation->TabIndex = 11;
			this->chkEnviromentMapSaturation->Text = L"Enviroment Map Saturation";
			this->tipMain->SetToolTip(this->chkEnviromentMapSaturation, L"$envmapsaturation");
			// 
			// chkEnviromentMapContrast
			// 
			this->chkEnviromentMapContrast->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->chkEnviromentMapContrast->Location = System::Drawing::Point(10, 185);
			this->chkEnviromentMapContrast->Name = L"chkEnviromentMapContrast";
			this->chkEnviromentMapContrast->Size = System::Drawing::Size(215, 27);
			this->chkEnviromentMapContrast->TabIndex = 10;
			this->chkEnviromentMapContrast->Text = L"Enviroment Map Contrast";
			this->tipMain->SetToolTip(this->chkEnviromentMapContrast, L"$envmapcontrast");
			// 
			// chkAdditive
			// 
			this->chkAdditive->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->chkAdditive->Location = System::Drawing::Point(10, 129);
			this->chkAdditive->Name = L"chkAdditive";
			this->chkAdditive->Size = System::Drawing::Size(192, 28);
			this->chkAdditive->TabIndex = 8;
			this->chkAdditive->Text = L"Additive";
			this->tipMain->SetToolTip(this->chkAdditive, L"$additive");
			// 
			// chkTranslucent
			// 
			this->chkTranslucent->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->chkTranslucent->Location = System::Drawing::Point(270, 157);
			this->chkTranslucent->Name = L"chkTranslucent";
			this->chkTranslucent->Size = System::Drawing::Size(192, 28);
			this->chkTranslucent->TabIndex = 15;
			this->chkTranslucent->Text = L"Translucent";
			this->tipMain->SetToolTip(this->chkTranslucent, L"$translucent");
			// 
			// txtKeywords
			// 
			this->txtKeywords->Location = System::Drawing::Point(115, 102);
			this->txtKeywords->Name = L"txtKeywords";
			this->txtKeywords->Size = System::Drawing::Size(347, 26);
			this->txtKeywords->TabIndex = 7;
			this->tipMain->SetToolTip(this->txtKeywords, L"%keywords");
			// 
			// lblKeywords
			// 
			this->lblKeywords->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->lblKeywords->Location = System::Drawing::Point(10, 102);
			this->lblKeywords->Name = L"lblKeywords";
			this->lblKeywords->Size = System::Drawing::Size(105, 26);
			this->lblKeywords->TabIndex = 6;
			this->lblKeywords->Text = L"Keywords:";
			// 
			// cboSurface2
			// 
			this->cboSurface2->Items->AddRange(gcnew cli::array< System::Object^  >(42) {
				L"AlienFlesh", L"ArmorFlesh", L"BloodyFlesh",
					L"Boulder", L"Brick", L"Chain", L"Chainlink", L"Computer", L"Concrete", L"Concrete_Block", L"Default", L"Dirt", L"Flesh", L"Glass",
					L"Grass", L"Gravel", L"Ice", L"Ladder", L"Metal_Box", L"Metal", L"MetalGrate", L"MetalPanel", L"MetalVent", L"MudSlipperySlime",
					L"Porcelain", L"Quicksand", L"Rock", L"Slime", L"SlipperyMetal", L"Snow", L"SolidMetal", L"Tile", L"Wade", L"Water", L"Watermelon",
					L"Wood_Box", L"Wood_Crate", L"Wood_Furniture", L"Wood_Panel", L"Wood_Plank", L"Wood_Solid", L"WoodWood_Lowdensity"
			});
			this->cboSurface2->Location = System::Drawing::Point(115, 74);
			this->cboSurface2->Name = L"cboSurface2";
			this->cboSurface2->Size = System::Drawing::Size(347, 28);
			this->cboSurface2->TabIndex = 5;
			this->tipMain->SetToolTip(this->cboSurface2, L"$surfaceprop2");
			// 
			// lblSurface2
			// 
			this->lblSurface2->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->lblSurface2->Location = System::Drawing::Point(10, 74);
			this->lblSurface2->Name = L"lblSurface2";
			this->lblSurface2->Size = System::Drawing::Size(105, 26);
			this->lblSurface2->TabIndex = 4;
			this->lblSurface2->Text = L"Surface 2:";
			// 
			// cboSurface1
			// 
			this->cboSurface1->Items->AddRange(gcnew cli::array< System::Object^  >(42) {
				L"AlienFlesh", L"ArmorFlesh", L"BloodyFlesh",
					L"Boulder", L"Brick", L"Chain", L"Chainlink", L"Computer", L"Concrete", L"Concrete_Block", L"Default", L"Dirt", L"Flesh", L"Glass",
					L"Grass", L"Gravel", L"Ice", L"Ladder", L"Metal_Box", L"Metal", L"MetalGrate", L"MetalPanel", L"MetalVent", L"MudSlipperySlime",
					L"Porcelain", L"Quicksand", L"Rock", L"Slime", L"SlipperyMetal", L"Snow", L"SolidMetal", L"Tile", L"Wade", L"Water", L"Watermelon",
					L"Wood_Box", L"Wood_Crate", L"Wood_Furniture", L"Wood_Panel", L"Wood_Plank", L"Wood_Solid", L"WoodWood_Lowdensity"
			});
			this->cboSurface1->Location = System::Drawing::Point(115, 46);
			this->cboSurface1->Name = L"cboSurface1";
			this->cboSurface1->Size = System::Drawing::Size(347, 28);
			this->cboSurface1->TabIndex = 3;
			this->tipMain->SetToolTip(this->cboSurface1, L"$surfaceprop");
			// 
			// lblSurface1
			// 
			this->lblSurface1->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->lblSurface1->Location = System::Drawing::Point(10, 46);
			this->lblSurface1->Name = L"lblSurface1";
			this->lblSurface1->Size = System::Drawing::Size(105, 27);
			this->lblSurface1->TabIndex = 2;
			this->lblSurface1->Text = L"Surface 1:";
			// 
			// cboShader
			// 
			this->cboShader->Items->AddRange(gcnew cli::array< System::Object^  >(13) {
				L"Cable", L"Decal", L"DecalModulate", L"LightmappedGeneric",
					L"Modulate", L"MonitorScreen", L"Predator", L"Refract", L"ShatteredGlass", L"Sprite", L"UnlitGeneric", L"VertexlitGeneric", L"Water"
			});
			this->cboShader->Location = System::Drawing::Point(115, 18);
			this->cboShader->Name = L"cboShader";
			this->cboShader->Size = System::Drawing::Size(347, 28);
			this->cboShader->TabIndex = 1;
			this->cboShader->Text = L"LightmappedGeneric";
			this->cboShader->TextChanged += gcnew System::EventHandler(this, &CVMTCreate::cboShader_TextChanged);
			// 
			// lblShader
			// 
			this->lblShader->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->lblShader->Location = System::Drawing::Point(10, 18);
			this->lblShader->Name = L"lblShader";
			this->lblShader->Size = System::Drawing::Size(105, 27);
			this->lblShader->TabIndex = 0;
			this->lblShader->Text = L"Shader:";
			// 
			// btnClose
			// 
			this->btnClose->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->btnClose->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->btnClose->Location = System::Drawing::Point(432, 366);
			this->btnClose->Name = L"btnClose";
			this->btnClose->Size = System::Drawing::Size(96, 27);
			this->btnClose->TabIndex = 2;
			this->btnClose->Text = L"&Close";
			this->btnClose->Click += gcnew System::EventHandler(this, &CVMTCreate::btnClose_Click);
			// 
			// btnCreate
			// 
			this->btnCreate->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->btnCreate->Location = System::Drawing::Point(330, 366);
			this->btnCreate->Name = L"btnCreate";
			this->btnCreate->Size = System::Drawing::Size(96, 27);
			this->btnCreate->TabIndex = 1;
			this->btnCreate->Text = L"C&reate";
			this->btnCreate->Click += gcnew System::EventHandler(this, &CVMTCreate::btnCreate_Click);
			// 
			// btnClear
			// 
			this->btnClear->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->btnClear->Location = System::Drawing::Point(24, 366);
			this->btnClear->Name = L"btnClear";
			this->btnClear->Size = System::Drawing::Size(96, 27);
			this->btnClear->TabIndex = 3;
			this->btnClear->Text = L"C&lear";
			this->btnClear->Click += gcnew System::EventHandler(this, &CVMTCreate::btnClear_Click);
			// 
			// dlgSaveVMTFile
			// 
			this->dlgSaveVMTFile->Filter = L"VMT Files (*.vmt)|*.vmt";
			this->dlgSaveVMTFile->Title = L"Save VMT File";
			// 
			// CVMTCreate
			// 
			this->AcceptButton = this->btnCreate;
			this->AutoScaleBaseSize = System::Drawing::Size(8, 19);
			this->CancelButton = this->btnClose;
			this->ClientSize = System::Drawing::Size(540, 405);
			this->Controls->Add(this->btnClear);
			this->Controls->Add(this->btnClose);
			this->Controls->Add(this->btnCreate);
			this->Controls->Add(this->tabMain);
			this->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8));
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"CVMTCreate";
			this->ShowInTaskbar = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"Create VMT File";
			this->grpTextures->ResumeLayout(false);
			this->grpTextures->PerformLayout();
			this->tabMain->ResumeLayout(false);
			this->tabTextures->ResumeLayout(false);
			this->tabOptions->ResumeLayout(false);
			this->grpOptions->ResumeLayout(false);
			this->grpOptions->PerformLayout();
			this->ResumeLayout(false);

		}
		private: System::Void GetTexture(System::Windows::Forms::TextBox ^oTextBox)
		{
			if(this->dlgOpenVTFFile->ShowDialog() == System::Windows::Forms::DialogResult::OK)
			{
				oTextBox->Text = CVMTFileUtility::GetTexturePathFromSystemPath(this->dlgOpenVTFFile->FileName);
			}
		}

		public: System::Void Create(System::String ^sFileName, VTFLib::CVTFFile &VTFFile)
		{
			this->txtBaseTexture1->Text = CVMTFileUtility::GetTexturePathFromSystemPath(sFileName);
			this->chkAlphaTest->Checked = VTFFile.GetFlag(TEXTUREFLAGS_ONEBITALPHA) != 0;
			this->chkTranslucent->Checked = VTFFile.GetFlag(TEXTUREFLAGS_EIGHTBITALPHA) != 0;
		}

		private: System::Void Clear()
		{
			this->txtBaseTexture1->Clear();
			this->txtBumpTexture1->Clear();
			this->txtBaseTexture2->Clear();
			this->txtBumpTexture2->Clear();
			this->txtEnviromentTexture->Clear();
			this->txtEnviromentMaskTexture->Clear();
			this->txtDetailTexture->Clear();
			this->txtToolTexture->Clear();
			this->txtNormalTexture->Clear();
			this->txtDuDvTexture->Clear();

			this->cboShader->Text = "LightmappedGeneric";
			this->cboSurface1->Text = "";
			this->cboSurface2->Text = "";

			this->txtKeywords->Clear();

			this->chkAdditive->Checked = false;
			this->chkAlphaTest->Checked = false;
			this->chkEnviromentMapContrast->Checked = false;
			this->chkEnviromentMapSaturation->Checked = false;
			this->chkNoCull->Checked = false;
			this->chkNoDecal->Checked = false;
			this->chkNoLOD->Checked = false;
			this->chkTranslucent->Checked = false;
			this->chkVertexAlpha->Checked = false;
			this->chkVertexColor->Checked = false;
		}

		private: System::Void txtTexture_Leave(System::Object ^  sender, System::EventArgs ^  e)
		{
			static_cast<System::Windows::Forms::TextBox ^>(sender)->Text = CVMTFileUtility::GetTexturePathFromSystemPath(static_cast<System::Windows::Forms::TextBox ^>(sender)->Text);
		}

		private: System::Void txtBaseTexture1_TextChanged(System::Object ^  sender, System::EventArgs ^  e)
		{
			//this->btnCreate->Enabled = this->GetTexturePath(txtBaseTexture1->Text)->Length != 0 && this->cboShader->Text->Length != 0;
		}

		private: System::Void cboShader_TextChanged(System::Object ^  sender, System::EventArgs ^  e)
		{
			this->btnCreate->Enabled =/* this->GetTexturePath(txtBaseTexture1->Text)->Length != 0 && */this->cboShader->Text->Length != 0;
		}

		private: System::Void btnBaseTexture1_Click(System::Object ^  sender, System::EventArgs ^  e)
		{
			this->GetTexture(this->txtBaseTexture1);
		}

		private: System::Void btnBumpTexture1_Click(System::Object ^  sender, System::EventArgs ^  e)
		{
			this->GetTexture(this->txtBumpTexture1);
		}

		private: System::Void btnBaseTexture2_Click(System::Object ^  sender, System::EventArgs ^  e)
		{
			this->GetTexture(this->txtBaseTexture2);
		}
		private: System::Void btnBumpTexture2_Click(System::Object ^  sender, System::EventArgs ^  e)
		{
			this->GetTexture(this->txtBumpTexture2);
		}

		private: System::Void btnEnviromentTexture_Click(System::Object ^  sender, System::EventArgs ^  e)
		{
			this->GetTexture(this->txtEnviromentTexture);
		}

		private: System::Void btnEnviromentMaskTexture_Click(System::Object ^  sender, System::EventArgs ^  e)
		{
			this->GetTexture(this->txtEnviromentMaskTexture);
		}

		private: System::Void btnDetailTexture_Click(System::Object ^  sender, System::EventArgs ^  e)
		{
			this->GetTexture(this->txtDetailTexture);
		}

		private: System::Void btnToolTexture_Click(System::Object ^  sender, System::EventArgs ^  e)
		{
			this->GetTexture(this->txtToolTexture);
		}

		private: System::Void btnNormalTexture_Click(System::Object ^  sender, System::EventArgs ^  e)
		{
			this->GetTexture(this->txtNormalTexture);
		}

		private: System::Void btnDuDvTexture_Click(System::Object ^  sender, System::EventArgs ^  e)
		{
			this->GetTexture(this->txtDuDvTexture);
		}

		private: System::Void btnClear_Click(System::Object ^  sender, System::EventArgs ^  e)
		{
			this->Clear();
		}

		private: System::Void AddVMTStringNode(VTFLib::CVMTFile &VMTFile, System::String ^sName, System::String ^sValue)
		{
			if(sValue->Length == 0)
				return;

			char *cName = (char *)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(sName).ToPointer();
			char *cValue = (char *)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(sValue).ToPointer();
			VMTFile.GetRoot()->AddStringNode(cName, cValue);
			System::Runtime::InteropServices::Marshal::FreeHGlobal((IntPtr)cValue);
			System::Runtime::InteropServices::Marshal::FreeHGlobal((IntPtr)cName);
		}

		private: System::Void AddVMTIntegerNode(VTFLib::CVMTFile &VMTFile, System::String ^sName, int iValue)
		{
			char *cName = (char *)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(sName).ToPointer();
			VMTFile.GetRoot()->AddIntegerNode(cName, iValue);
			System::Runtime::InteropServices::Marshal::FreeHGlobal((IntPtr)cName);
		}

		private: System::Void AddVMTSingleNode(VTFLib::CVMTFile &VMTFile, System::String ^sName, float fValue)
		{
			char *cName = (char *)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(sName).ToPointer();
			VMTFile.GetRoot()->AddSingleNode(cName, fValue);
			System::Runtime::InteropServices::Marshal::FreeHGlobal((IntPtr)cName);
		}

		private: System::Void AddVMTBooleanNode(VTFLib::CVMTFile &VMTFile, System::String ^sName, bool bValue, bool bDefault)
		{
			if(bValue == bDefault)
				return;

			char *cName = (char *)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(sName).ToPointer();
			VMTFile.GetRoot()->AddIntegerNode(cName, bValue ? 1 : 0);
			System::Runtime::InteropServices::Marshal::FreeHGlobal((IntPtr)cName);
		}

		private: System::Void btnCreate_Click(System::Object ^  sender, System::EventArgs ^  e)
		{
			System::String ^sVMTFile = this->txtBaseTexture1->Text;

			int iIndex = sVMTFile->LastIndexOf("/");
			if(iIndex != -1)
			{
				sVMTFile = sVMTFile->Substring(iIndex + 1);
			}

			this->dlgSaveVMTFile->FileName = sVMTFile;
			if(this->dlgSaveVMTFile->ShowDialog() != System::Windows::Forms::DialogResult::OK)
				return;

			char *cTemp;
			VTFLib::CVMTFile VMTFile;

			cTemp = (char *)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(this->cboShader->Text).ToPointer();
			VMTFile.Create(cTemp);
			System::Runtime::InteropServices::Marshal::FreeHGlobal((IntPtr)cTemp);

			this->AddVMTStringNode(VMTFile, "$basetexture", this->txtBaseTexture1->Text);
			this->AddVMTStringNode(VMTFile, "$bumpmap", this->txtBumpTexture1->Text);
			this->AddVMTStringNode(VMTFile, "$basetexture2", this->txtBaseTexture2->Text);
			this->AddVMTStringNode(VMTFile, "$bumpmap2", this->txtBumpTexture2->Text);
			this->AddVMTStringNode(VMTFile, "$envmap", this->txtEnviromentTexture->Text);
			this->AddVMTStringNode(VMTFile, "$envmapmask", this->txtEnviromentMaskTexture->Text);
			this->AddVMTStringNode(VMTFile, "$detail", this->txtDetailTexture->Text);
			this->AddVMTStringNode(VMTFile, "%tooltexture", this->txtToolTexture->Text);
			this->AddVMTStringNode(VMTFile, "$normalmap", this->txtNormalTexture->Text);
			this->AddVMTStringNode(VMTFile, "$dudvmap", this->txtDuDvTexture->Text);

			this->AddVMTStringNode(VMTFile, "$surfaceprop", this->cboSurface1->Text);
			this->AddVMTStringNode(VMTFile, "$surfaceprop2", this->cboSurface2->Text);

			this->AddVMTStringNode(VMTFile, "%keywords", this->txtKeywords->Text);

			this->AddVMTBooleanNode(VMTFile, "$additive", this->chkAdditive->Checked, false);
			this->AddVMTBooleanNode(VMTFile, "$alphatest", this->chkAlphaTest->Checked, false);
			this->AddVMTBooleanNode(VMTFile, "$envmapcontrast", this->chkEnviromentMapContrast->Checked, false);
			this->AddVMTBooleanNode(VMTFile, "$envmapsaturation", this->chkEnviromentMapSaturation->Checked, false);
			this->AddVMTBooleanNode(VMTFile, "$nocull", this->chkNoCull->Checked, false);
			this->AddVMTBooleanNode(VMTFile, "$nodecal", this->chkNoDecal->Checked, false);
			this->AddVMTBooleanNode(VMTFile, "$nolod", this->chkNoLOD->Checked, false);
			this->AddVMTBooleanNode(VMTFile, "$translucent", this->chkTranslucent->Checked, false);
			this->AddVMTBooleanNode(VMTFile, "$vertexalpha", this->chkVertexAlpha->Checked, false);
			this->AddVMTBooleanNode(VMTFile, "$vertexcolor", this->chkVertexColor->Checked, false);

			cTemp = (char *)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(this->dlgSaveVMTFile->FileName).ToPointer();
			VMTFile.Save(cTemp);
			System::Runtime::InteropServices::Marshal::FreeHGlobal((IntPtr)cTemp);
		}

		private: System::Void btnClose_Click(System::Object ^  sender, System::EventArgs ^  e)
		{
			this->DialogResult = System::Windows::Forms::DialogResult::OK;
		}
	private: System::Void grpOptions_Enter(System::Object^ sender, System::EventArgs^ e) {
	}
};
}