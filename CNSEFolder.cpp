//custom GUID in 'iconstatus.propdesc' (red square)
const PROPERTYKEY PKEY_StatusIconColumn = {{0xa00fe456,0x3e31,0x4811,{0xa3,0x67,0x00,0xe4,0x3a,0x8e,0x19,0xe2}},1};
const PROPERTYKEY PKEY_StatusIcon = {{0xa00fe456,0x3e31,0x4811,{0xa3,0x67,0x00,0xe4,0x3a,0x8e,0x19,0xe2}},2};

//multiple icon GUID. (in iconstatus.propdesc).
const PROPERTYKEY PKEY_StatusIcon2 = { {0xa00fe456,0x3e31,0x4811,{0xa3,0x67,0x00,0xe4,0x3a,0x8e,0x19,0xe2}},3 };
const PROPERTYKEY PKEY_StatusIcon3 = { {0xa00fe456,0x3e31,0x4811,{0xa3,0x67,0x00,0xe4,0x3a,0x8e,0x19,0xe2}},4 };

//undocumented property key.
//{c9944a21-a406-48fe-8225-aec7e24c211b}
const PROPERTYKEY PKEY_SystemStatusIcon = { { 0xc9944a21, 0xa406, 0x48fe, 0x82, 0x25, 0xae, 0xc7, 0xe2, 0x4c, 0x21, 0x1b }, 16 };
const PROPERTYKEY PKEY_SystemIconFlag = { { 0xC9944A21, 0xA406, 0x48FE, 0x82, 0x25, 0xAE, 0xC7, 0xE2, 0x4C, 0x21, 0x1B }, 18 };

//Mapping Column to own GUID
HRESULT __stdcall CNSEFolder::MapColumnToSCID(UINT iColumn, SHCOLUMNID *pscid)
{
	/* ... */
	if(iColumn is status icon column){
		*pscid = PKEY_StatusIconColumn;
	}
	/* ... */
	return S_OK;
}

HRESULT __stdcall CPropertyStore::GetValue(REFPROPERTYKEY key, PROPVARIANT *pv){ //IPropertyStore
	HRESULT hr = S_OK;
	/* ... */
	if(key == PKEY_StatusIconColumn){
		PROPVARIANT tmp;
		IPropertyStore* ppropstore = NULL;
		IPersistSerializedPropStorage* psps = NULL;
		DWORD cbSize = 0;
		BYTE* pBuf = NULL;
		
		if(!IsWindows8OrGreater()){ //support windows 8+ (in windows 7 explorer will crash)
			hr = E_FAIL;
			goto cleanArea;
		}
		hr = PSCreateMemoryPropertyStore(IID_IPropertyStore,&ppropstore);
		if(FAILED(hr)){
			goto cleanArea;
		}
		
		//initialize icon column (system)
		tmp.vt = VT_BSTR;
		tmp.bstrVal = SysAllocString(L"prop:NSE1.VF.Status;"); //(orange square in propdesc)
		ppropstore->SetValue(PKEY_SystemStatusIcon,&tmp);
		
		tmp.vt = VT_UI4;
		tmp.uintVal = 2;
		ppropstore->SetValue(PKEY_SystemIconFlag,&tmp);
		
		//icon 1
		tmp.vt = VT_UI4;
		tmp.uintVal = 1; //option 1 (in propdesc)
		//tmp.uintVal = 2; //option 2 (in propdesc)
		ppropstore->SetValue(PKEY_StatusIcon,&tmp);

		//icon 2
		tmp.vt = VT_UI4;
		tmp.uintVal = 1; //option 1 (in propdesc)
		//tmp.uintVal = 2; //option 2 (in propdesc)
		ppropstore->SetValue(PKEY_StatusIcon2, &tmp);

		//icon 3
		tmp.vt = VT_UI4;
		tmp.uintVal = 1; //option 1 (in propdesc)
		//tmp.uintVal = 2; //option 2 (in propdesc)
		ppropstore->SetValue(PKEY_StatusIcon3, &tmp);
		
		hr = ps->QueryInterface(IID_IPersistSerializedPropStorage,&psps);
		if(FAILED(hr)){
			goto cleanArea;
		}
		
		hr = psps->GetPropertyStorage(&pBuf,&cbSize);
		if(FAILED(hr)){
			goto cleanArea;
		}
		
		pv->vt = VT_BLOB;
		pv->blob.pBlobData = pBuf;
        pv->blob.cbSize = cbSize;
		
		cleanArea:
		if(psps){
			psps->Release();
		}
		if(ppropstore){
			ppropstore->Release();
		}
	}
	/* ... */
	return hr;
}	