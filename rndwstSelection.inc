#if defined _include_rndwstSelection
	#error There is an '#include <rndwstSelection>' definition more than once! 
#else
	#define _include_rndwstSelection
#endif

#define RNDWST_SELECTION_LOOP(%0,%1) for(new %0 = 0; %0 < %1; %0++)

#if !defined MAX_RNDWSTSELECTION_ITEM
	#define MAX_RNDWSTSELECTION_ITEM (200)
#endif

#if !defined MAX_RNDWSTSELECTION_NAME
	#define MAX_RNDWSTSELECTION_NAME (64)
#endif

#if !defined MAX_RNDWSTSELECTION_DESC
	#define MAX_RNDWSTSELECTION_DESC (128)
#endif

#if !defined MAX_RNDWSTSELECTION_HEADER
	#define MAX_RNDWSTSELECTION_HEADER (64)
#endif

#define RESPONSE_RNDWSTSELECTION_CANCEL (0)
#define RESPONSE_RNDWSTSELECTION_YES (1)

#define Selection:%0(%1) \
	forward selection_%0(%1); public selection_%0(%1)

#define ShowPlayerSelection(%0,%1, \
	rndwstOpenSelection(%0, #%1,

enum RNDWST_SELECTION_ITEM
{
	rndwstSelection_Model,
	rndwstSelection_Amount,
	rndwstSelection_Name[MAX_RNDWSTSELECTION_NAME],
	rndwstSelection_Desc[MAX_RNDWSTSELECTION_DESC],
	rndwstSelection_Color[2],

	Float:rndwstSelection_RotX,
	Float:rndwstSelection_RotY,
	Float:rndwstSelection_RotZ,
	Float:rndwstSelection_Zoom,
}
new rndwstSelectionInfo[MAX_PLAYERS][MAX_RNDWSTSELECTION_ITEM][RNDWST_SELECTION_ITEM];

new bool:rndwstSelection_player_opened[MAX_PLAYERS],
	bool:rndwstSelection_player_loaded[MAX_PLAYERS] = false,
	rndwstSelection_player_minpage[MAX_PLAYERS],
	rndwstSelection_player_maxpage[MAX_PLAYERS],
	rndwstSelection_player_page[MAX_PLAYERS],
	rndwstSelection_player_btncolor[MAX_PLAYERS],
	rndwstSelection_player_header[MAX_PLAYERS][MAX_RNDWSTSELECTION_HEADER],
	rndwstSelection_player_selected[MAX_PLAYERS],
	rndwstSelection_player_selmodel[MAX_PLAYERS],
	rndwstSelection_player_selcolor[MAX_PLAYERS][2],
	rndwstSelection_player_name[MAX_PLAYERS][32 char];

new PlayerText:rndwstSelection_box[MAX_PLAYERS],
	PlayerText:rndwstSelection_infoitem_box[MAX_PLAYERS],
	PlayerText:rndwstSelection_header[MAX_PLAYERS],
	PlayerText:rndwstSelection_nextpage[MAX_PLAYERS],
	PlayerText:rndwstSelection_page[MAX_PLAYERS],
	PlayerText:rndwstSelection_previouspage[MAX_PLAYERS],
	PlayerText:rndwstSelection_infoitem_model[MAX_PLAYERS],
	PlayerText:rndwstSelection_infoitem_name[MAX_PLAYERS],
	PlayerText:rndwstSelection_infoitem_desc[MAX_PLAYERS],
	PlayerText:rndwstSelection_close[MAX_PLAYERS],
	PlayerText:rndwstSelection_infoitem_select[MAX_PLAYERS],
	PlayerText:rndwstSelection_item_model[MAX_PLAYERS][15],
	PlayerText:rndwstSelection_item_name[MAX_PLAYERS][15],
	PlayerText:rndwstSelection_item_amount[MAX_PLAYERS][15];

public OnPlayerConnect(playerid)
{
	rndwstSelection_ClearItem(playerid);
	rndwstSelection_player_loaded[playerid] = false;
	rndwstSelection_LoadTextDraw(playerid);
	return CallLocalFunction("RN_OnPlayerConnect", "d", playerid);
}

public OnPlayerDisconnect(playerid, reason)
{
	rndwstSelection_ClearItem(playerid);
	rndwstSelection_DestroyTextDraw(playerid);
	return CallLocalFunction("RN_OnPlayerDisconnect", "dd", playerid, reason);
}

public OnPlayerClickTextDraw(playerid, Text:clickedid)
{
	if(rndwstSelection_player_opened[playerid] == true) 
	{
		if(clickedid == Text:INVALID_TEXT_DRAW)
		{
			rndwstSelection_Close(playerid);

			new
				string[40];

			strcat(string, "selection_");
			strcat(string, rndwstSelection_player_name[playerid]);
			CallLocalFunction(string, "ddddd", playerid, RESPONSE_RNDWSTSELECTION_CANCEL, rndwstSelection_player_selmodel[playerid], rndwstSelection_player_selcolor[playerid][0], rndwstSelection_player_selcolor[playerid][1]);
		}
	}	
    return CallLocalFunction("RN_OnPlayerClickTextDraw", "dd", playerid, _:clickedid);
}

public OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid)
{
	if(rndwstSelection_player_opened[playerid] == true) 
	{
		if(playertextid == PlayerText:INVALID_TEXT_DRAW)
		{
			rndwstSelection_Close(playerid);

			new
				string[40];

			strcat(string, "selection_");
			strcat(string, rndwstSelection_player_name[playerid]);

			CallLocalFunction(string, "ddddd", playerid, RESPONSE_RNDWSTSELECTION_CANCEL, rndwstSelection_player_selmodel[playerid], rndwstSelection_player_selcolor[playerid][0], rndwstSelection_player_selcolor[playerid][1]);
		}
		else if(playertextid == rndwstSelection_close[playerid])
		{
			rndwstSelection_Close(playerid);
			
			new
				string[40];

			strcat(string, "selection_");
			strcat(string, rndwstSelection_player_name[playerid]);

			CallLocalFunction(string, "ddddd", playerid, RESPONSE_RNDWSTSELECTION_CANCEL, rndwstSelection_player_selmodel[playerid], rndwstSelection_player_selcolor[playerid][0], rndwstSelection_player_selcolor[playerid][1]);
		}
		else if(playertextid == rndwstSelection_previouspage[playerid])
		{
			if(rndwstSelection_player_page[playerid] == rndwstSelection_player_minpage[playerid]) return 1;
			else
			{
				rndwstSelection_player_page[playerid] = rndwstSelection_player_page[playerid] - 1;
				rndwstSelection_ShowItem(playerid, rndwstSelection_player_page[playerid]);
			}
		}
		else if(playertextid == rndwstSelection_nextpage[playerid])
		{
			if(rndwstSelection_player_page[playerid] == rndwstSelection_player_maxpage[playerid]) return 1;
			else
			{
				rndwstSelection_player_page[playerid] = rndwstSelection_player_page[playerid] + 1;
				rndwstSelection_ShowItem(playerid, rndwstSelection_player_page[playerid]);
			}
		}
		else if(playertextid == rndwstSelection_infoitem_select[playerid])
		{
			rndwstSelection_Close(playerid);

			new
				string[40];

			strcat(string, "selection_");
			strcat(string, rndwstSelection_player_name[playerid]);

			CallLocalFunction(string, "ddddd", playerid, RESPONSE_RNDWSTSELECTION_YES, rndwstSelection_player_selmodel[playerid], rndwstSelection_player_selcolor[playerid][0], rndwstSelection_player_selcolor[playerid][1]);
		}
		else 
		{
			new selectitemslot = -1;
			RNDWST_SELECTION_LOOP(i, 15)
			{
				if(playertextid == rndwstSelection_item_model[playerid][i])
				{
					selectitemslot = i;
					break;
				}
			}

			if(selectitemslot != -1)
			{
				rndwstSelection_SelectItem(playerid, selectitemslot);
			}
		}
	}
	return CallLocalFunction("RN_OnPlayerClickPlayerTextDraw", "dd", playerid, _:playertextid);
}

stock AddPlayerSelectionItem(playerid, model, const name[], const desc[])
{
	new slot = -1;
	RNDWST_SELECTION_LOOP(i, MAX_RNDWSTSELECTION_ITEM)
	{
		if(rndwstSelectionInfo[playerid][i][rndwstSelection_Model] == -1) 
		{
			slot = i;
			break;
		}
	}

	if(slot == -1) 
		return printf("[WARNING (rndwstSelection)] The maximum MAX_RNDWSTSELECTION_ITEM is %d, please increase it to add more models", MAX_RNDWSTSELECTION_ITEM);

	rndwstSelectionInfo[playerid][slot][rndwstSelection_Model] = model;
	rndwstSelectionInfo[playerid][slot][rndwstSelection_Amount] = -1;
	format(rndwstSelectionInfo[playerid][slot][rndwstSelection_Name], MAX_RNDWSTSELECTION_NAME + 1, "%s", name);
	format(rndwstSelectionInfo[playerid][slot][rndwstSelection_Desc], MAX_RNDWSTSELECTION_DESC + 1, "%s", desc);

	rndwstSelectionInfo[playerid][slot][rndwstSelection_Color][0] = 1;
	rndwstSelectionInfo[playerid][slot][rndwstSelection_Color][1] = 1;

	rndwstSelectionInfo[playerid][slot][rndwstSelection_RotX] = -10.000000;
	rndwstSelectionInfo[playerid][slot][rndwstSelection_RotY] = 0.000000;
	rndwstSelectionInfo[playerid][slot][rndwstSelection_RotZ] = -20.000000;
	rndwstSelectionInfo[playerid][slot][rndwstSelection_Zoom] = 1.000000;
	return slot;
}

stock SetPlayerSelectionItemAmount(playerid, slot, amount)
{
	if(slot == -1) return 1;
	rndwstSelectionInfo[playerid][slot][rndwstSelection_Amount] = amount;
	return 1;
}

stock SetPlayerSelectionVehColor(playerid, slot, vehicle_color1, vehicle_color2)
{
	if(slot == -1) return 1;
	rndwstSelectionInfo[playerid][slot][rndwstSelection_Color][0] = vehicle_color1;
	rndwstSelectionInfo[playerid][slot][rndwstSelection_Color][1] = vehicle_color2;
	return slot;
}

stock SetPlayerSelectionItemRot(playerid, slot, Float:rotx, Float:roty, Float:rotz, Float:zoom)
{
	if(slot == -1) return 1;
	rndwstSelectionInfo[playerid][slot][rndwstSelection_RotX] = rotx;
	rndwstSelectionInfo[playerid][slot][rndwstSelection_RotY] = roty;
	rndwstSelectionInfo[playerid][slot][rndwstSelection_RotZ] = rotz;
	rndwstSelectionInfo[playerid][slot][rndwstSelection_Zoom] = zoom;
	return slot;
}

stock rndwstOpenSelection(playerid, const function[], const header[], const backbtn[] = "<<<", const nextbtn[] = ">>>", btncolor = -8388353)
{
	rndwstSelection_player_page[playerid] = 1;
	rndwstSelection_player_minpage[playerid] = 1;
	rndwstSelection_player_opened[playerid] = true;
	rndwstSelection_player_btncolor[playerid] = btncolor;
	format(rndwstSelection_player_header[playerid], MAX_RNDWSTSELECTION_HEADER, header);
	rndwstSelection_player_selected[playerid] = -1;
	strpack(rndwstSelection_player_name[playerid], function, 32 char);

	new max_page, item_count = 0, page_fmt[100];

	RNDWST_SELECTION_LOOP(i, MAX_RNDWSTSELECTION_ITEM)
	{
		if(rndwstSelectionInfo[playerid][i][rndwstSelection_Model] != -1)
		{
			item_count++;
		}
	}

	if(item_count % 15 == 0)
	{
		max_page = item_count / 15;
	}
	else 
	{
		max_page = (item_count / 15) + 1;
	}

	rndwstSelection_player_maxpage[playerid] = max_page;

	PlayerTextDrawSetString(playerid, rndwstSelection_header[playerid], header);
	format(page_fmt, sizeof(page_fmt), "%d/%d", rndwstSelection_player_page[playerid], rndwstSelection_player_maxpage[playerid]);
	PlayerTextDrawSetString(playerid, rndwstSelection_page[playerid], page_fmt);

	PlayerTextDrawSetString(playerid, rndwstSelection_previouspage[playerid], backbtn);
	PlayerTextDrawSetString(playerid, rndwstSelection_nextpage[playerid], nextbtn);

	PlayerTextDrawShow(playerid, rndwstSelection_box[playerid]);
	PlayerTextDrawShow(playerid, rndwstSelection_infoitem_box[playerid]);
	PlayerTextDrawShow(playerid, rndwstSelection_header[playerid]);
	PlayerTextDrawShow(playerid, rndwstSelection_nextpage[playerid]);
	PlayerTextDrawShow(playerid, rndwstSelection_page[playerid]);
	PlayerTextDrawShow(playerid, rndwstSelection_previouspage[playerid]);
	PlayerTextDrawShow(playerid, rndwstSelection_close[playerid]);

	rndwstSelection_SelectItem(playerid, rndwstSelection_player_selected[playerid]);
	rndwstSelection_ShowItem(playerid, rndwstSelection_player_page[playerid]);
	SelectTextDraw(playerid, btncolor);
	return 1;
}

stock rndwstSelection_ClearItem(playerid)
{
	RNDWST_SELECTION_LOOP(i, MAX_RNDWSTSELECTION_ITEM)
	{
		rndwstSelectionInfo[playerid][i][rndwstSelection_Model] = -1;
	}
	return 1;
}

stock rndwstSelection_ShowItem(playerid, page) 
{
	new jumlah_item = 15, min_slot, max_slot;

	RNDWST_SELECTION_LOOP(i, jumlah_item)
	{
		PlayerTextDrawBackgroundColor(playerid, rndwstSelection_item_model[playerid][i], 125);
		PlayerTextDrawHide(playerid, rndwstSelection_item_model[playerid][i]);
		PlayerTextDrawHide(playerid, rndwstSelection_item_name[playerid][i]);
		PlayerTextDrawHide(playerid, rndwstSelection_item_amount[playerid][i]);
	}

	min_slot = (jumlah_item * page) - jumlah_item;
	max_slot = (jumlah_item * page);

	for(new i = min_slot; i < max_slot; i++)
	{
		if(rndwstSelectionInfo[playerid][i][rndwstSelection_Model] != -1)
		{
			PlayerTextDrawSetPreviewVehCol(playerid, rndwstSelection_item_model[playerid][i - min_slot], rndwstSelectionInfo[playerid][i][rndwstSelection_Color][0], rndwstSelectionInfo[playerid][i][rndwstSelection_Color][1]);
			PlayerTextDrawSetPreviewModel(playerid, rndwstSelection_item_model[playerid][i - min_slot], rndwstSelectionInfo[playerid][i][rndwstSelection_Model]);
			PlayerTextDrawSetPreviewRot(playerid, rndwstSelection_item_model[playerid][i - min_slot], rndwstSelectionInfo[playerid][i][rndwstSelection_RotX], rndwstSelectionInfo[playerid][i][rndwstSelection_RotY], rndwstSelectionInfo[playerid][i][rndwstSelection_RotZ], rndwstSelectionInfo[playerid][i][rndwstSelection_Zoom]);
			PlayerTextDrawSetString(playerid, rndwstSelection_item_name[playerid][i - min_slot], rndwstSelectionInfo[playerid][i][rndwstSelection_Name]);
			PlayerTextDrawShow(playerid, rndwstSelection_item_model[playerid][i - min_slot]);
			PlayerTextDrawShow(playerid, rndwstSelection_item_name[playerid][i - min_slot]);

			if(rndwstSelectionInfo[playerid][i][rndwstSelection_Amount] != -1)
			{
				new fix[20];
				format(fix, sizeof(fix), "%d", rndwstSelectionInfo[playerid][i][rndwstSelection_Amount]);
				PlayerTextDrawSetString(playerid, rndwstSelection_item_amount[playerid][i - min_slot], fix);
				PlayerTextDrawShow(playerid, rndwstSelection_item_amount[playerid][i - min_slot]);
			}
		}
	}

	new page_fmt[50];
	format(page_fmt, sizeof(page_fmt), "%d/%d", rndwstSelection_player_page[playerid], rndwstSelection_player_maxpage[playerid]);
	PlayerTextDrawSetString(playerid, rndwstSelection_page[playerid], page_fmt);
	return 1;
}

stock rndwstSelection_SelectItem(playerid, slot)
{
	if(rndwstSelection_player_opened[playerid] == false) return 1;

	if(slot == -1)
	{
		PlayerTextDrawHide(playerid, rndwstSelection_infoitem_select[playerid]);
		PlayerTextDrawHide(playerid, rndwstSelection_infoitem_model[playerid]);
		PlayerTextDrawHide(playerid, rndwstSelection_infoitem_name[playerid]);
		PlayerTextDrawHide(playerid, rndwstSelection_infoitem_desc[playerid]);
	}
	else 
	{
		new jumlah_item = 15, min_slot;
		min_slot = (jumlah_item * rndwstSelection_player_page[playerid]) - jumlah_item;
		
		PlayerTextDrawSetPreviewModel(playerid, rndwstSelection_infoitem_model[playerid], rndwstSelectionInfo[playerid][slot + min_slot][rndwstSelection_Model]);
		PlayerTextDrawSetPreviewRot(playerid, rndwstSelection_infoitem_model[playerid], rndwstSelectionInfo[playerid][slot + min_slot][rndwstSelection_RotX], rndwstSelectionInfo[playerid][slot + min_slot][rndwstSelection_RotY], rndwstSelectionInfo[playerid][slot + min_slot][rndwstSelection_RotZ], rndwstSelectionInfo[playerid][slot + min_slot][rndwstSelection_Zoom]);
		PlayerTextDrawSetPreviewVehCol(playerid, rndwstSelection_infoitem_model[playerid], rndwstSelectionInfo[playerid][slot + min_slot][rndwstSelection_Color][0], rndwstSelectionInfo[playerid][slot + min_slot][rndwstSelection_Color][1]);
		PlayerTextDrawSetString(playerid, rndwstSelection_infoitem_name[playerid], rndwstSelectionInfo[playerid][slot + min_slot][rndwstSelection_Name]);
		PlayerTextDrawSetString(playerid, rndwstSelection_infoitem_desc[playerid], rndwstSelectionInfo[playerid][slot + min_slot][rndwstSelection_Desc]);

		PlayerTextDrawShow(playerid, rndwstSelection_infoitem_select[playerid]);
		PlayerTextDrawShow(playerid, rndwstSelection_infoitem_model[playerid]);
		PlayerTextDrawShow(playerid, rndwstSelection_infoitem_name[playerid]);
		PlayerTextDrawShow(playerid, rndwstSelection_infoitem_desc[playerid]);

		rndwstSelection_player_selmodel[playerid] = rndwstSelectionInfo[playerid][slot + min_slot][rndwstSelection_Model];
		rndwstSelection_player_selcolor[playerid][0] = rndwstSelectionInfo[playerid][slot + min_slot][rndwstSelection_Color][0];
		rndwstSelection_player_selcolor[playerid][1] = rndwstSelectionInfo[playerid][slot + min_slot][rndwstSelection_Color][1];

		if(rndwstSelection_player_selected[playerid] != -1 && rndwstSelection_player_selected[playerid] != slot)
		{
			if(rndwstSelectionInfo[playerid][(rndwstSelection_player_page[playerid] - 1) * 15 + rndwstSelection_player_selected[playerid]][rndwstSelection_Model] == -1)
			{
				PlayerTextDrawBackgroundColor(playerid, rndwstSelection_item_model[playerid][rndwstSelection_player_selected[playerid]], 125);
				PlayerTextDrawHide(playerid, rndwstSelection_item_model[playerid][rndwstSelection_player_selected[playerid]]);
			}
			else 
			{
				PlayerTextDrawBackgroundColor(playerid, rndwstSelection_item_model[playerid][rndwstSelection_player_selected[playerid]], 125);
				PlayerTextDrawShow(playerid, rndwstSelection_item_model[playerid][rndwstSelection_player_selected[playerid]]);
			}
		}
		PlayerTextDrawBackgroundColor(playerid, rndwstSelection_item_model[playerid][slot], rndwstSelection_player_btncolor[playerid]);
		PlayerTextDrawShow(playerid, rndwstSelection_item_model[playerid][slot]);
	}

	rndwstSelection_player_selected[playerid] = slot;
	return 1;
}

stock rndwstSelection_Close(playerid)
{
	if(rndwstSelection_player_opened[playerid] == false) return 1;

	PlayerTextDrawHide(playerid, rndwstSelection_box[playerid]);
	PlayerTextDrawHide(playerid, rndwstSelection_infoitem_box[playerid]);
	PlayerTextDrawHide(playerid, rndwstSelection_header[playerid]);
	PlayerTextDrawHide(playerid, rndwstSelection_nextpage[playerid]);
	PlayerTextDrawHide(playerid, rndwstSelection_page[playerid]);
	PlayerTextDrawHide(playerid, rndwstSelection_previouspage[playerid]);
	PlayerTextDrawHide(playerid, rndwstSelection_infoitem_model[playerid]);
	PlayerTextDrawHide(playerid, rndwstSelection_infoitem_name[playerid]);
	PlayerTextDrawHide(playerid, rndwstSelection_infoitem_desc[playerid]);
	PlayerTextDrawHide(playerid, rndwstSelection_close[playerid]);
	PlayerTextDrawHide(playerid, rndwstSelection_infoitem_select[playerid]);
	
	RNDWST_SELECTION_LOOP(i, 15) 
	{
		PlayerTextDrawBackgroundColor(playerid, rndwstSelection_item_model[playerid][i], 125);
		PlayerTextDrawHide(playerid, rndwstSelection_item_model[playerid][i]);
		PlayerTextDrawHide(playerid, rndwstSelection_item_name[playerid][i]);
		PlayerTextDrawHide(playerid, rndwstSelection_item_amount[playerid][i]);
	}

	CancelSelectTextDraw(playerid);
	rndwstSelection_ClearItem(playerid);
	rndwstSelection_player_opened[playerid] = false;
	return 1;
}

stock rndwstSelection_DestroyTextDraw(playerid)
{
	rndwstSelection_player_loaded[playerid] = false;

	PlayerTextDrawDestroy(playerid, rndwstSelection_box[playerid]);
	PlayerTextDrawDestroy(playerid, rndwstSelection_infoitem_box[playerid]);
	PlayerTextDrawDestroy(playerid, rndwstSelection_header[playerid]);
	PlayerTextDrawDestroy(playerid, rndwstSelection_nextpage[playerid]);
	PlayerTextDrawDestroy(playerid, rndwstSelection_page[playerid]);
	PlayerTextDrawDestroy(playerid, rndwstSelection_previouspage[playerid]);
	PlayerTextDrawDestroy(playerid, rndwstSelection_infoitem_model[playerid]);
	PlayerTextDrawDestroy(playerid, rndwstSelection_infoitem_name[playerid]);
	PlayerTextDrawDestroy(playerid, rndwstSelection_infoitem_desc[playerid]);
	PlayerTextDrawDestroy(playerid, rndwstSelection_close[playerid]);
	PlayerTextDrawDestroy(playerid, rndwstSelection_infoitem_select[playerid]);
	
	RNDWST_SELECTION_LOOP(i, 15) 
	{
		PlayerTextDrawDestroy(playerid, rndwstSelection_item_model[playerid][i]);
		PlayerTextDrawDestroy(playerid, rndwstSelection_item_name[playerid][i]);
		PlayerTextDrawDestroy(playerid, rndwstSelection_item_amount[playerid][i]);
	}
	return 1;
}

stock rndwstSelection_LoadTextDraw(playerid)
{
	rndwstSelection_box[playerid] = CreatePlayerTextDraw(playerid, 325.000000, 141.000000, "_");
	PlayerTextDrawFont(playerid, rndwstSelection_box[playerid], 1);
	PlayerTextDrawLetterSize(playerid, rndwstSelection_box[playerid], 0.600000, 23.749958);
	PlayerTextDrawTextSize(playerid, rndwstSelection_box[playerid], 298.500000, 370.000000);
	PlayerTextDrawSetOutline(playerid, rndwstSelection_box[playerid], 1);
	PlayerTextDrawSetShadow(playerid, rndwstSelection_box[playerid], 0);
	PlayerTextDrawAlignment(playerid, rndwstSelection_box[playerid], 2);
	PlayerTextDrawColor(playerid, rndwstSelection_box[playerid], -1);
	PlayerTextDrawBackgroundColor(playerid, rndwstSelection_box[playerid], 255);
	PlayerTextDrawBoxColor(playerid, rndwstSelection_box[playerid], 135);
	PlayerTextDrawUseBox(playerid, rndwstSelection_box[playerid], 1);
	PlayerTextDrawSetProportional(playerid, rndwstSelection_box[playerid], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstSelection_box[playerid], 0);

	rndwstSelection_infoitem_box[playerid] = CreatePlayerTextDraw(playerid, 188.000000, 167.000000, "_");
	PlayerTextDrawFont(playerid, rndwstSelection_infoitem_box[playerid], 1);
	PlayerTextDrawLetterSize(playerid, rndwstSelection_infoitem_box[playerid], 0.600000, 17.349990);
	PlayerTextDrawTextSize(playerid, rndwstSelection_infoitem_box[playerid], 298.500000, 87.000000);
	PlayerTextDrawSetOutline(playerid, rndwstSelection_infoitem_box[playerid], 1);
	PlayerTextDrawSetShadow(playerid, rndwstSelection_infoitem_box[playerid], 0);
	PlayerTextDrawAlignment(playerid, rndwstSelection_infoitem_box[playerid], 2);
	PlayerTextDrawColor(playerid, rndwstSelection_infoitem_box[playerid], -1);
	PlayerTextDrawBackgroundColor(playerid, rndwstSelection_infoitem_box[playerid], 255);
	PlayerTextDrawBoxColor(playerid, rndwstSelection_infoitem_box[playerid], 135);
	PlayerTextDrawUseBox(playerid, rndwstSelection_infoitem_box[playerid], 1);
	PlayerTextDrawSetProportional(playerid, rndwstSelection_infoitem_box[playerid], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstSelection_infoitem_box[playerid], 0);

	rndwstSelection_header[playerid] = CreatePlayerTextDraw(playerid, 143.000000, 143.000000, "SELECTION");
	PlayerTextDrawFont(playerid, rndwstSelection_header[playerid], 1);
	PlayerTextDrawLetterSize(playerid, rndwstSelection_header[playerid], 0.550000, 1.850000);
	PlayerTextDrawTextSize(playerid, rndwstSelection_header[playerid], 400.000000, 17.000000);
	PlayerTextDrawSetOutline(playerid, rndwstSelection_header[playerid], 0);
	PlayerTextDrawSetShadow(playerid, rndwstSelection_header[playerid], 1);
	PlayerTextDrawAlignment(playerid, rndwstSelection_header[playerid], 1);
	PlayerTextDrawColor(playerid, rndwstSelection_header[playerid], -1);
	PlayerTextDrawBackgroundColor(playerid, rndwstSelection_header[playerid], 255);
	PlayerTextDrawBoxColor(playerid, rndwstSelection_header[playerid], 50);
	PlayerTextDrawUseBox(playerid, rndwstSelection_header[playerid], 0);
	PlayerTextDrawSetProportional(playerid, rndwstSelection_header[playerid], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstSelection_header[playerid], 0);
	
	rndwstSelection_nextpage[playerid] = CreatePlayerTextDraw(playerid, 451.000000, 333.000000, "Next Page");
	PlayerTextDrawFont(playerid, rndwstSelection_nextpage[playerid], 2);
	PlayerTextDrawLetterSize(playerid, rndwstSelection_nextpage[playerid], 0.258332, 1.750000);
	PlayerTextDrawTextSize(playerid, rndwstSelection_nextpage[playerid], 16.500000, 90.500000);
	PlayerTextDrawSetOutline(playerid, rndwstSelection_nextpage[playerid], 1);
	PlayerTextDrawSetShadow(playerid, rndwstSelection_nextpage[playerid], 0);
	PlayerTextDrawAlignment(playerid, rndwstSelection_nextpage[playerid], 2);
	PlayerTextDrawColor(playerid, rndwstSelection_nextpage[playerid], -1);
	PlayerTextDrawBackgroundColor(playerid, rndwstSelection_nextpage[playerid], 255);
	PlayerTextDrawBoxColor(playerid, rndwstSelection_nextpage[playerid], 200);
	PlayerTextDrawUseBox(playerid, rndwstSelection_nextpage[playerid], 1);
	PlayerTextDrawSetProportional(playerid, rndwstSelection_nextpage[playerid], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstSelection_nextpage[playerid], 1);

	rndwstSelection_page[playerid] = CreatePlayerTextDraw(playerid, 374.000000, 333.000000, "1/10");
	PlayerTextDrawFont(playerid, rndwstSelection_page[playerid], 2);
	PlayerTextDrawLetterSize(playerid, rndwstSelection_page[playerid], 0.258332, 1.750000);
	PlayerTextDrawTextSize(playerid, rndwstSelection_page[playerid], 16.500000, 90.500000);
	PlayerTextDrawSetOutline(playerid, rndwstSelection_page[playerid], 1);
	PlayerTextDrawSetShadow(playerid, rndwstSelection_page[playerid], 0);
	PlayerTextDrawAlignment(playerid, rndwstSelection_page[playerid], 2);
	PlayerTextDrawColor(playerid, rndwstSelection_page[playerid], -1);
	PlayerTextDrawBackgroundColor(playerid, rndwstSelection_page[playerid], 255);
	PlayerTextDrawBoxColor(playerid, rndwstSelection_page[playerid], 200);
	PlayerTextDrawUseBox(playerid, rndwstSelection_page[playerid], 0);
	PlayerTextDrawSetProportional(playerid, rndwstSelection_page[playerid], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstSelection_page[playerid], 0);

	rndwstSelection_previouspage[playerid] = CreatePlayerTextDraw(playerid, 299.000000, 333.000000, "Previous Page");
	PlayerTextDrawFont(playerid, rndwstSelection_previouspage[playerid], 2);
	PlayerTextDrawLetterSize(playerid, rndwstSelection_previouspage[playerid], 0.258332, 1.750000);
	PlayerTextDrawTextSize(playerid, rndwstSelection_previouspage[playerid], 16.500000, 90.500000);
	PlayerTextDrawSetOutline(playerid, rndwstSelection_previouspage[playerid], 1);
	PlayerTextDrawSetShadow(playerid, rndwstSelection_previouspage[playerid], 0);
	PlayerTextDrawAlignment(playerid, rndwstSelection_previouspage[playerid], 2);
	PlayerTextDrawColor(playerid, rndwstSelection_previouspage[playerid], -1);
	PlayerTextDrawBackgroundColor(playerid, rndwstSelection_previouspage[playerid], 255);
	PlayerTextDrawBoxColor(playerid, rndwstSelection_previouspage[playerid], 200);
	PlayerTextDrawUseBox(playerid, rndwstSelection_previouspage[playerid], 1);
	PlayerTextDrawSetProportional(playerid, rndwstSelection_previouspage[playerid], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstSelection_previouspage[playerid], 1);

	rndwstSelection_infoitem_model[playerid] = CreatePlayerTextDraw(playerid, 143.000000, 167.000000, "Preview_Model");
	PlayerTextDrawFont(playerid, rndwstSelection_infoitem_model[playerid], 5);
	PlayerTextDrawLetterSize(playerid, rndwstSelection_infoitem_model[playerid], 0.600000, 2.000000);
	PlayerTextDrawTextSize(playerid, rndwstSelection_infoitem_model[playerid], 89.500000, 84.500000);
	PlayerTextDrawSetOutline(playerid, rndwstSelection_infoitem_model[playerid], 0);
	PlayerTextDrawSetShadow(playerid, rndwstSelection_infoitem_model[playerid], 0);
	PlayerTextDrawAlignment(playerid, rndwstSelection_infoitem_model[playerid], 2);
	PlayerTextDrawColor(playerid, rndwstSelection_infoitem_model[playerid], -1);
	PlayerTextDrawBackgroundColor(playerid, rndwstSelection_infoitem_model[playerid], 0);
	PlayerTextDrawBoxColor(playerid, rndwstSelection_infoitem_model[playerid], 255);
	PlayerTextDrawUseBox(playerid, rndwstSelection_infoitem_model[playerid], 0);
	PlayerTextDrawSetProportional(playerid, rndwstSelection_infoitem_model[playerid], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstSelection_infoitem_model[playerid], 0);
	PlayerTextDrawSetPreviewModel(playerid, rndwstSelection_infoitem_model[playerid], 19094);
	PlayerTextDrawSetPreviewRot(playerid, rndwstSelection_infoitem_model[playerid], -10.000000, 0.000000, -20.000000, 1.000000);
	PlayerTextDrawSetPreviewVehCol(playerid, rndwstSelection_infoitem_model[playerid], 1, 1);

	rndwstSelection_infoitem_name[playerid] = CreatePlayerTextDraw(playerid, 147.000000, 168.000000, "Burger");
	PlayerTextDrawFont(playerid, rndwstSelection_infoitem_name[playerid], 1);
	PlayerTextDrawLetterSize(playerid, rndwstSelection_infoitem_name[playerid], 0.237500, 1.200000);
	PlayerTextDrawTextSize(playerid, rndwstSelection_infoitem_name[playerid], 229.500000, 17.000000);
	PlayerTextDrawSetOutline(playerid, rndwstSelection_infoitem_name[playerid], 0);
	PlayerTextDrawSetShadow(playerid, rndwstSelection_infoitem_name[playerid], 0);
	PlayerTextDrawAlignment(playerid, rndwstSelection_infoitem_name[playerid], 1);
	PlayerTextDrawColor(playerid, rndwstSelection_infoitem_name[playerid], -1);
	PlayerTextDrawBackgroundColor(playerid, rndwstSelection_infoitem_name[playerid], 255);
	PlayerTextDrawBoxColor(playerid, rndwstSelection_infoitem_name[playerid], 50);
	PlayerTextDrawUseBox(playerid, rndwstSelection_infoitem_name[playerid], 0);
	PlayerTextDrawSetProportional(playerid, rndwstSelection_infoitem_name[playerid], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstSelection_infoitem_name[playerid], 0);

	rndwstSelection_infoitem_desc[playerid] = CreatePlayerTextDraw(playerid, 147.000000, 243.000000, "Dapat mengurangi rasa lapar karakter dan menambah darah karakter jika darah karakter kurang dari 100%");
	PlayerTextDrawFont(playerid, rndwstSelection_infoitem_desc[playerid], 1);
	PlayerTextDrawLetterSize(playerid, rndwstSelection_infoitem_desc[playerid], 0.154166, 0.750000);
	PlayerTextDrawTextSize(playerid, rndwstSelection_infoitem_desc[playerid], 229.500000, 17.000000);
	PlayerTextDrawSetOutline(playerid, rndwstSelection_infoitem_desc[playerid], 0);
	PlayerTextDrawSetShadow(playerid, rndwstSelection_infoitem_desc[playerid], 0);
	PlayerTextDrawAlignment(playerid, rndwstSelection_infoitem_desc[playerid], 1);
	PlayerTextDrawColor(playerid, rndwstSelection_infoitem_desc[playerid], -1);
	PlayerTextDrawBackgroundColor(playerid, rndwstSelection_infoitem_desc[playerid], 255);
	PlayerTextDrawBoxColor(playerid, rndwstSelection_infoitem_desc[playerid], 50);
	PlayerTextDrawUseBox(playerid, rndwstSelection_infoitem_desc[playerid], 0);
	PlayerTextDrawSetProportional(playerid, rndwstSelection_infoitem_desc[playerid], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstSelection_infoitem_desc[playerid], 0);

	rndwstSelection_close[playerid] = CreatePlayerTextDraw(playerid, 494.000000, 130.000000, "ld_beat:cross");
	PlayerTextDrawFont(playerid, rndwstSelection_close[playerid], 4);
	PlayerTextDrawLetterSize(playerid, rndwstSelection_close[playerid], 0.600000, 2.000000);
	PlayerTextDrawTextSize(playerid, rndwstSelection_close[playerid], 26.000000, 26.500000);
	PlayerTextDrawSetOutline(playerid, rndwstSelection_close[playerid], 1);
	PlayerTextDrawSetShadow(playerid, rndwstSelection_close[playerid], 0);
	PlayerTextDrawAlignment(playerid, rndwstSelection_close[playerid], 1);
	PlayerTextDrawColor(playerid, rndwstSelection_close[playerid], -16776961);
	PlayerTextDrawBackgroundColor(playerid, rndwstSelection_close[playerid], 255);
	PlayerTextDrawBoxColor(playerid, rndwstSelection_close[playerid], 50);
	PlayerTextDrawUseBox(playerid, rndwstSelection_close[playerid], 1);
	PlayerTextDrawSetProportional(playerid, rndwstSelection_close[playerid], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstSelection_close[playerid], 1);

	rndwstSelection_infoitem_select[playerid] = CreatePlayerTextDraw(playerid, 188.000000, 333.000000, "Select");
	PlayerTextDrawFont(playerid, rndwstSelection_infoitem_select[playerid], 2);
	PlayerTextDrawLetterSize(playerid, rndwstSelection_infoitem_select[playerid], 0.258332, 1.750000);
	PlayerTextDrawTextSize(playerid, rndwstSelection_infoitem_select[playerid], 16.500000, 87.000000);
	PlayerTextDrawSetOutline(playerid, rndwstSelection_infoitem_select[playerid], 1);
	PlayerTextDrawSetShadow(playerid, rndwstSelection_infoitem_select[playerid], 0);
	PlayerTextDrawAlignment(playerid, rndwstSelection_infoitem_select[playerid], 2);
	PlayerTextDrawColor(playerid, rndwstSelection_infoitem_select[playerid], -1);
	PlayerTextDrawBackgroundColor(playerid, rndwstSelection_infoitem_select[playerid], 255);
	PlayerTextDrawBoxColor(playerid, rndwstSelection_infoitem_select[playerid], -2686776);
	PlayerTextDrawUseBox(playerid, rndwstSelection_infoitem_select[playerid], 1);
	PlayerTextDrawSetProportional(playerid, rndwstSelection_infoitem_select[playerid], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstSelection_infoitem_select[playerid], 1);

	rndwstSelection_item_model[playerid][0] = CreatePlayerTextDraw(playerid, 237.000000, 165.000000, "Preview_Model");
	PlayerTextDrawFont(playerid, rndwstSelection_item_model[playerid][0], 5);
	PlayerTextDrawLetterSize(playerid, rndwstSelection_item_model[playerid][0], 0.600000, 2.000000);
	PlayerTextDrawTextSize(playerid, rndwstSelection_item_model[playerid][0], 50.000000, 50.000000);
	PlayerTextDrawSetOutline(playerid, rndwstSelection_item_model[playerid][0], 0);
	PlayerTextDrawSetShadow(playerid, rndwstSelection_item_model[playerid][0], 0);
	PlayerTextDrawAlignment(playerid, rndwstSelection_item_model[playerid][0], 2);
	PlayerTextDrawColor(playerid, rndwstSelection_item_model[playerid][0], -1);
	PlayerTextDrawBackgroundColor(playerid, rndwstSelection_item_model[playerid][0], 125);
	PlayerTextDrawBoxColor(playerid, rndwstSelection_item_model[playerid][0], 255);
	PlayerTextDrawUseBox(playerid, rndwstSelection_item_model[playerid][0], 0);
	PlayerTextDrawSetProportional(playerid, rndwstSelection_item_model[playerid][0], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstSelection_item_model[playerid][0], 1);
	PlayerTextDrawSetPreviewModel(playerid, rndwstSelection_item_model[playerid][0], 19094);
	PlayerTextDrawSetPreviewRot(playerid, rndwstSelection_item_model[playerid][0], -10.000000, 0.000000, -20.000000, 1.000000);
	PlayerTextDrawSetPreviewVehCol(playerid, rndwstSelection_item_model[playerid][0], 1, 1);
	
	rndwstSelection_item_model[playerid][1] = CreatePlayerTextDraw(playerid, 292.000000, 165.000000, "Preview_Model");
	PlayerTextDrawFont(playerid, rndwstSelection_item_model[playerid][1], 5);
	PlayerTextDrawLetterSize(playerid, rndwstSelection_item_model[playerid][1], 0.600000, 2.000000);
	PlayerTextDrawTextSize(playerid, rndwstSelection_item_model[playerid][1], 50.000000, 50.000000);
	PlayerTextDrawSetOutline(playerid, rndwstSelection_item_model[playerid][1], 0);
	PlayerTextDrawSetShadow(playerid, rndwstSelection_item_model[playerid][1], 0);
	PlayerTextDrawAlignment(playerid, rndwstSelection_item_model[playerid][1], 1);
	PlayerTextDrawColor(playerid, rndwstSelection_item_model[playerid][1], -1);
	PlayerTextDrawBackgroundColor(playerid, rndwstSelection_item_model[playerid][1], 125);
	PlayerTextDrawBoxColor(playerid, rndwstSelection_item_model[playerid][1], 255);
	PlayerTextDrawUseBox(playerid, rndwstSelection_item_model[playerid][1], 0);
	PlayerTextDrawSetProportional(playerid, rndwstSelection_item_model[playerid][1], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstSelection_item_model[playerid][1], 1);
	PlayerTextDrawSetPreviewModel(playerid, rndwstSelection_item_model[playerid][1], 19094);
	PlayerTextDrawSetPreviewRot(playerid, rndwstSelection_item_model[playerid][1], -10.000000, 0.000000, -20.000000, 1.000000);
	PlayerTextDrawSetPreviewVehCol(playerid, rndwstSelection_item_model[playerid][1], 1, 1);

	rndwstSelection_item_model[playerid][2] = CreatePlayerTextDraw(playerid, 347.000000, 165.000000, "Preview_Model");
	PlayerTextDrawFont(playerid, rndwstSelection_item_model[playerid][2], 5);
	PlayerTextDrawLetterSize(playerid, rndwstSelection_item_model[playerid][2], 0.600000, 2.000000);
	PlayerTextDrawTextSize(playerid, rndwstSelection_item_model[playerid][2], 50.000000, 50.000000);
	PlayerTextDrawSetOutline(playerid, rndwstSelection_item_model[playerid][2], 0);
	PlayerTextDrawSetShadow(playerid, rndwstSelection_item_model[playerid][2], 0);
	PlayerTextDrawAlignment(playerid, rndwstSelection_item_model[playerid][2], 1);
	PlayerTextDrawColor(playerid, rndwstSelection_item_model[playerid][2], -1);
	PlayerTextDrawBackgroundColor(playerid, rndwstSelection_item_model[playerid][2], 125);
	PlayerTextDrawBoxColor(playerid, rndwstSelection_item_model[playerid][2], 255);
	PlayerTextDrawUseBox(playerid, rndwstSelection_item_model[playerid][2], 0);
	PlayerTextDrawSetProportional(playerid, rndwstSelection_item_model[playerid][2], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstSelection_item_model[playerid][2], 1);
	PlayerTextDrawSetPreviewModel(playerid, rndwstSelection_item_model[playerid][2], 19094);
	PlayerTextDrawSetPreviewRot(playerid, rndwstSelection_item_model[playerid][2], -10.000000, 0.000000, -20.000000, 1.000000);
	PlayerTextDrawSetPreviewVehCol(playerid, rndwstSelection_item_model[playerid][2], 1, 1);

	rndwstSelection_item_model[playerid][3] = CreatePlayerTextDraw(playerid, 402.000000, 165.000000, "Preview_Model");
	PlayerTextDrawFont(playerid, rndwstSelection_item_model[playerid][3], 5);
	PlayerTextDrawLetterSize(playerid, rndwstSelection_item_model[playerid][3], 0.600000, 2.000000);
	PlayerTextDrawTextSize(playerid, rndwstSelection_item_model[playerid][3], 50.000000, 50.000000);
	PlayerTextDrawSetOutline(playerid, rndwstSelection_item_model[playerid][3], 0);
	PlayerTextDrawSetShadow(playerid, rndwstSelection_item_model[playerid][3], 0);
	PlayerTextDrawAlignment(playerid, rndwstSelection_item_model[playerid][3], 1);
	PlayerTextDrawColor(playerid, rndwstSelection_item_model[playerid][3], -1);
	PlayerTextDrawBackgroundColor(playerid, rndwstSelection_item_model[playerid][3], 125);
	PlayerTextDrawBoxColor(playerid, rndwstSelection_item_model[playerid][3], 255);
	PlayerTextDrawUseBox(playerid, rndwstSelection_item_model[playerid][3], 0);
	PlayerTextDrawSetProportional(playerid, rndwstSelection_item_model[playerid][3], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstSelection_item_model[playerid][3], 1);
	PlayerTextDrawSetPreviewModel(playerid, rndwstSelection_item_model[playerid][3], 19094);
	PlayerTextDrawSetPreviewRot(playerid, rndwstSelection_item_model[playerid][3], -10.000000, 0.000000, -20.000000, 1.000000);
	PlayerTextDrawSetPreviewVehCol(playerid, rndwstSelection_item_model[playerid][3], 1, 1);

	rndwstSelection_item_model[playerid][4] = CreatePlayerTextDraw(playerid, 457.000000, 165.000000, "Preview_Model");
	PlayerTextDrawFont(playerid, rndwstSelection_item_model[playerid][4], 5);
	PlayerTextDrawLetterSize(playerid, rndwstSelection_item_model[playerid][4], 0.600000, 2.000000);
	PlayerTextDrawTextSize(playerid, rndwstSelection_item_model[playerid][4], 50.000000, 50.000000);
	PlayerTextDrawSetOutline(playerid, rndwstSelection_item_model[playerid][4], 0);
	PlayerTextDrawSetShadow(playerid, rndwstSelection_item_model[playerid][4], 0);
	PlayerTextDrawAlignment(playerid, rndwstSelection_item_model[playerid][4], 1);
	PlayerTextDrawColor(playerid, rndwstSelection_item_model[playerid][4], -1);
	PlayerTextDrawBackgroundColor(playerid, rndwstSelection_item_model[playerid][4], 125);
	PlayerTextDrawBoxColor(playerid, rndwstSelection_item_model[playerid][4], 255);
	PlayerTextDrawUseBox(playerid, rndwstSelection_item_model[playerid][4], 0);
	PlayerTextDrawSetProportional(playerid, rndwstSelection_item_model[playerid][4], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstSelection_item_model[playerid][4], 1);
	PlayerTextDrawSetPreviewModel(playerid, rndwstSelection_item_model[playerid][4], 19094);
	PlayerTextDrawSetPreviewRot(playerid, rndwstSelection_item_model[playerid][4], -10.000000, 0.000000, -20.000000, 1.000000);
	PlayerTextDrawSetPreviewVehCol(playerid, rndwstSelection_item_model[playerid][4], 1, 1);

	rndwstSelection_item_model[playerid][5] = CreatePlayerTextDraw(playerid, 237.000000, 220.000000, "Preview_Model");
	PlayerTextDrawFont(playerid, rndwstSelection_item_model[playerid][5], 5);
	PlayerTextDrawLetterSize(playerid, rndwstSelection_item_model[playerid][5], 0.600000, 2.000000);
	PlayerTextDrawTextSize(playerid, rndwstSelection_item_model[playerid][5], 50.000000, 50.000000);
	PlayerTextDrawSetOutline(playerid, rndwstSelection_item_model[playerid][5], 0);
	PlayerTextDrawSetShadow(playerid, rndwstSelection_item_model[playerid][5], 0);
	PlayerTextDrawAlignment(playerid, rndwstSelection_item_model[playerid][5], 1);
	PlayerTextDrawColor(playerid, rndwstSelection_item_model[playerid][5], -1);
	PlayerTextDrawBackgroundColor(playerid, rndwstSelection_item_model[playerid][5], 125);
	PlayerTextDrawBoxColor(playerid, rndwstSelection_item_model[playerid][5], 255);
	PlayerTextDrawUseBox(playerid, rndwstSelection_item_model[playerid][5], 0);
	PlayerTextDrawSetProportional(playerid, rndwstSelection_item_model[playerid][5], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstSelection_item_model[playerid][5], 1);
	PlayerTextDrawSetPreviewModel(playerid, rndwstSelection_item_model[playerid][5], 19094);
	PlayerTextDrawSetPreviewRot(playerid, rndwstSelection_item_model[playerid][5], -10.000000, 0.000000, -20.000000, 1.000000);
	PlayerTextDrawSetPreviewVehCol(playerid, rndwstSelection_item_model[playerid][5], 1, 1);

	rndwstSelection_item_model[playerid][6] = CreatePlayerTextDraw(playerid, 292.000000, 220.000000, "Preview_Model");
	PlayerTextDrawFont(playerid, rndwstSelection_item_model[playerid][6], 5);
	PlayerTextDrawLetterSize(playerid, rndwstSelection_item_model[playerid][6], 0.600000, 2.000000);
	PlayerTextDrawTextSize(playerid, rndwstSelection_item_model[playerid][6], 50.000000, 50.000000);
	PlayerTextDrawSetOutline(playerid, rndwstSelection_item_model[playerid][6], 0);
	PlayerTextDrawSetShadow(playerid, rndwstSelection_item_model[playerid][6], 0);
	PlayerTextDrawAlignment(playerid, rndwstSelection_item_model[playerid][6], 1);
	PlayerTextDrawColor(playerid, rndwstSelection_item_model[playerid][6], -1);
	PlayerTextDrawBackgroundColor(playerid, rndwstSelection_item_model[playerid][6], 125);
	PlayerTextDrawBoxColor(playerid, rndwstSelection_item_model[playerid][6], 255);
	PlayerTextDrawUseBox(playerid, rndwstSelection_item_model[playerid][6], 0);
	PlayerTextDrawSetProportional(playerid, rndwstSelection_item_model[playerid][6], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstSelection_item_model[playerid][6], 1);
	PlayerTextDrawSetPreviewModel(playerid, rndwstSelection_item_model[playerid][6], 19094);
	PlayerTextDrawSetPreviewRot(playerid, rndwstSelection_item_model[playerid][6], -10.000000, 0.000000, -20.000000, 1.000000);
	PlayerTextDrawSetPreviewVehCol(playerid, rndwstSelection_item_model[playerid][6], 1, 1);

	rndwstSelection_item_model[playerid][7] = CreatePlayerTextDraw(playerid, 347.000000, 220.000000, "Preview_Model");
	PlayerTextDrawFont(playerid, rndwstSelection_item_model[playerid][7], 5);
	PlayerTextDrawLetterSize(playerid, rndwstSelection_item_model[playerid][7], 0.600000, 2.000000);
	PlayerTextDrawTextSize(playerid, rndwstSelection_item_model[playerid][7], 50.000000, 50.000000);
	PlayerTextDrawSetOutline(playerid, rndwstSelection_item_model[playerid][7], 0);
	PlayerTextDrawSetShadow(playerid, rndwstSelection_item_model[playerid][7], 0);
	PlayerTextDrawAlignment(playerid, rndwstSelection_item_model[playerid][7], 1);
	PlayerTextDrawColor(playerid, rndwstSelection_item_model[playerid][7], -1);
	PlayerTextDrawBackgroundColor(playerid, rndwstSelection_item_model[playerid][7], 125);
	PlayerTextDrawBoxColor(playerid, rndwstSelection_item_model[playerid][7], 255);
	PlayerTextDrawUseBox(playerid, rndwstSelection_item_model[playerid][7], 0);
	PlayerTextDrawSetProportional(playerid, rndwstSelection_item_model[playerid][7], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstSelection_item_model[playerid][7], 1);
	PlayerTextDrawSetPreviewModel(playerid, rndwstSelection_item_model[playerid][7], 19094);
	PlayerTextDrawSetPreviewRot(playerid, rndwstSelection_item_model[playerid][7], -10.000000, 0.000000, -20.000000, 1.000000);
	PlayerTextDrawSetPreviewVehCol(playerid, rndwstSelection_item_model[playerid][7], 1, 1);

	rndwstSelection_item_model[playerid][8] = CreatePlayerTextDraw(playerid, 402.000000, 220.000000, "Preview_Model");
	PlayerTextDrawFont(playerid, rndwstSelection_item_model[playerid][8], 5);
	PlayerTextDrawLetterSize(playerid, rndwstSelection_item_model[playerid][8], 0.600000, 2.000000);
	PlayerTextDrawTextSize(playerid, rndwstSelection_item_model[playerid][8], 50.000000, 50.000000);
	PlayerTextDrawSetOutline(playerid, rndwstSelection_item_model[playerid][8], 0);
	PlayerTextDrawSetShadow(playerid, rndwstSelection_item_model[playerid][8], 0);
	PlayerTextDrawAlignment(playerid, rndwstSelection_item_model[playerid][8], 1);
	PlayerTextDrawColor(playerid, rndwstSelection_item_model[playerid][8], -1);
	PlayerTextDrawBackgroundColor(playerid, rndwstSelection_item_model[playerid][8], 125);
	PlayerTextDrawBoxColor(playerid, rndwstSelection_item_model[playerid][8], 255);
	PlayerTextDrawUseBox(playerid, rndwstSelection_item_model[playerid][8], 0);
	PlayerTextDrawSetProportional(playerid, rndwstSelection_item_model[playerid][8], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstSelection_item_model[playerid][8], 1);
	PlayerTextDrawSetPreviewModel(playerid, rndwstSelection_item_model[playerid][8], 19094);
	PlayerTextDrawSetPreviewRot(playerid, rndwstSelection_item_model[playerid][8], -10.000000, 0.000000, -20.000000, 1.000000);
	PlayerTextDrawSetPreviewVehCol(playerid, rndwstSelection_item_model[playerid][8], 1, 1);

	rndwstSelection_item_model[playerid][9] = CreatePlayerTextDraw(playerid, 457.000000, 220.000000, "Preview_Model");
	PlayerTextDrawFont(playerid, rndwstSelection_item_model[playerid][9], 5);
	PlayerTextDrawLetterSize(playerid, rndwstSelection_item_model[playerid][9], 0.600000, 2.000000);
	PlayerTextDrawTextSize(playerid, rndwstSelection_item_model[playerid][9], 50.000000, 50.000000);
	PlayerTextDrawSetOutline(playerid, rndwstSelection_item_model[playerid][9], 0);
	PlayerTextDrawSetShadow(playerid, rndwstSelection_item_model[playerid][9], 0);
	PlayerTextDrawAlignment(playerid, rndwstSelection_item_model[playerid][9], 1);
	PlayerTextDrawColor(playerid, rndwstSelection_item_model[playerid][9], -1);
	PlayerTextDrawBackgroundColor(playerid, rndwstSelection_item_model[playerid][9], 125);
	PlayerTextDrawBoxColor(playerid, rndwstSelection_item_model[playerid][9], 255);
	PlayerTextDrawUseBox(playerid, rndwstSelection_item_model[playerid][9], 0);
	PlayerTextDrawSetProportional(playerid, rndwstSelection_item_model[playerid][9], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstSelection_item_model[playerid][9], 1);
	PlayerTextDrawSetPreviewModel(playerid, rndwstSelection_item_model[playerid][9], 19094);
	PlayerTextDrawSetPreviewRot(playerid, rndwstSelection_item_model[playerid][9], -10.000000, 0.000000, -20.000000, 1.000000);
	PlayerTextDrawSetPreviewVehCol(playerid, rndwstSelection_item_model[playerid][9], 1, 1);

	rndwstSelection_item_model[playerid][10] = CreatePlayerTextDraw(playerid, 237.000000, 275.000000, "Preview_Model");
	PlayerTextDrawFont(playerid, rndwstSelection_item_model[playerid][10], 5);
	PlayerTextDrawLetterSize(playerid, rndwstSelection_item_model[playerid][10], 0.600000, 2.000000);
	PlayerTextDrawTextSize(playerid, rndwstSelection_item_model[playerid][10], 50.000000, 50.000000);
	PlayerTextDrawSetOutline(playerid, rndwstSelection_item_model[playerid][10], 0);
	PlayerTextDrawSetShadow(playerid, rndwstSelection_item_model[playerid][10], 0);
	PlayerTextDrawAlignment(playerid, rndwstSelection_item_model[playerid][10], 1);
	PlayerTextDrawColor(playerid, rndwstSelection_item_model[playerid][10], -1);
	PlayerTextDrawBackgroundColor(playerid, rndwstSelection_item_model[playerid][10], 125);
	PlayerTextDrawBoxColor(playerid, rndwstSelection_item_model[playerid][10], 255);
	PlayerTextDrawUseBox(playerid, rndwstSelection_item_model[playerid][10], 0);
	PlayerTextDrawSetProportional(playerid, rndwstSelection_item_model[playerid][10], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstSelection_item_model[playerid][10], 1);
	PlayerTextDrawSetPreviewModel(playerid, rndwstSelection_item_model[playerid][10], 19094);
	PlayerTextDrawSetPreviewRot(playerid, rndwstSelection_item_model[playerid][10], -10.000000, 0.000000, -20.000000, 1.000000);
	PlayerTextDrawSetPreviewVehCol(playerid, rndwstSelection_item_model[playerid][10], 1, 1);

	rndwstSelection_item_model[playerid][11] = CreatePlayerTextDraw(playerid, 292.000000, 275.000000, "Preview_Model");
	PlayerTextDrawFont(playerid, rndwstSelection_item_model[playerid][11], 5);
	PlayerTextDrawLetterSize(playerid, rndwstSelection_item_model[playerid][11], 0.600000, 2.000000);
	PlayerTextDrawTextSize(playerid, rndwstSelection_item_model[playerid][11], 50.000000, 50.000000);
	PlayerTextDrawSetOutline(playerid, rndwstSelection_item_model[playerid][11], 0);
	PlayerTextDrawSetShadow(playerid, rndwstSelection_item_model[playerid][11], 0);
	PlayerTextDrawAlignment(playerid, rndwstSelection_item_model[playerid][11], 1);
	PlayerTextDrawColor(playerid, rndwstSelection_item_model[playerid][11], -1);
	PlayerTextDrawBackgroundColor(playerid, rndwstSelection_item_model[playerid][11], 125);
	PlayerTextDrawBoxColor(playerid, rndwstSelection_item_model[playerid][11], 255);
	PlayerTextDrawUseBox(playerid, rndwstSelection_item_model[playerid][11], 0);
	PlayerTextDrawSetProportional(playerid, rndwstSelection_item_model[playerid][11], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstSelection_item_model[playerid][11], 1);
	PlayerTextDrawSetPreviewModel(playerid, rndwstSelection_item_model[playerid][11], 19094);
	PlayerTextDrawSetPreviewRot(playerid, rndwstSelection_item_model[playerid][11], -10.000000, 0.000000, -20.000000, 1.000000);
	PlayerTextDrawSetPreviewVehCol(playerid, rndwstSelection_item_model[playerid][11], 1, 1);

	rndwstSelection_item_model[playerid][12] = CreatePlayerTextDraw(playerid, 347.000000, 275.000000, "Preview_Model");
	PlayerTextDrawFont(playerid, rndwstSelection_item_model[playerid][12], 5);
	PlayerTextDrawLetterSize(playerid, rndwstSelection_item_model[playerid][12], 0.600000, 2.000000);
	PlayerTextDrawTextSize(playerid, rndwstSelection_item_model[playerid][12], 50.000000, 50.000000);
	PlayerTextDrawSetOutline(playerid, rndwstSelection_item_model[playerid][12], 0);
	PlayerTextDrawSetShadow(playerid, rndwstSelection_item_model[playerid][12], 0);
	PlayerTextDrawAlignment(playerid, rndwstSelection_item_model[playerid][12], 1);
	PlayerTextDrawColor(playerid, rndwstSelection_item_model[playerid][12], -1);
	PlayerTextDrawBackgroundColor(playerid, rndwstSelection_item_model[playerid][12], 125);
	PlayerTextDrawBoxColor(playerid, rndwstSelection_item_model[playerid][12], 255);
	PlayerTextDrawUseBox(playerid, rndwstSelection_item_model[playerid][12], 0);
	PlayerTextDrawSetProportional(playerid, rndwstSelection_item_model[playerid][12], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstSelection_item_model[playerid][12], 1);
	PlayerTextDrawSetPreviewModel(playerid, rndwstSelection_item_model[playerid][12], 19094);
	PlayerTextDrawSetPreviewRot(playerid, rndwstSelection_item_model[playerid][12], -10.000000, 0.000000, -20.000000, 1.000000);
	PlayerTextDrawSetPreviewVehCol(playerid, rndwstSelection_item_model[playerid][12], 1, 1);

	rndwstSelection_item_model[playerid][13] = CreatePlayerTextDraw(playerid, 402.000000, 275.000000, "Preview_Model");
	PlayerTextDrawFont(playerid, rndwstSelection_item_model[playerid][13], 5);
	PlayerTextDrawLetterSize(playerid, rndwstSelection_item_model[playerid][13], 0.600000, 2.000000);
	PlayerTextDrawTextSize(playerid, rndwstSelection_item_model[playerid][13], 50.000000, 50.000000);
	PlayerTextDrawSetOutline(playerid, rndwstSelection_item_model[playerid][13], 0);
	PlayerTextDrawSetShadow(playerid, rndwstSelection_item_model[playerid][13], 0);
	PlayerTextDrawAlignment(playerid, rndwstSelection_item_model[playerid][13], 1);
	PlayerTextDrawColor(playerid, rndwstSelection_item_model[playerid][13], -1);
	PlayerTextDrawBackgroundColor(playerid, rndwstSelection_item_model[playerid][13], 125);
	PlayerTextDrawBoxColor(playerid, rndwstSelection_item_model[playerid][13], 255);
	PlayerTextDrawUseBox(playerid, rndwstSelection_item_model[playerid][13], 0);
	PlayerTextDrawSetProportional(playerid, rndwstSelection_item_model[playerid][13], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstSelection_item_model[playerid][13], 1);
	PlayerTextDrawSetPreviewModel(playerid, rndwstSelection_item_model[playerid][13], 19094);
	PlayerTextDrawSetPreviewRot(playerid, rndwstSelection_item_model[playerid][13], -10.000000, 0.000000, -20.000000, 1.000000);
	PlayerTextDrawSetPreviewVehCol(playerid, rndwstSelection_item_model[playerid][13], 1, 1);

	rndwstSelection_item_model[playerid][14] = CreatePlayerTextDraw(playerid, 457.000000, 275.000000, "Preview_Model");
	PlayerTextDrawFont(playerid, rndwstSelection_item_model[playerid][14], 5);
	PlayerTextDrawLetterSize(playerid, rndwstSelection_item_model[playerid][14], 0.600000, 2.000000);
	PlayerTextDrawTextSize(playerid, rndwstSelection_item_model[playerid][14], 50.000000, 50.000000);
	PlayerTextDrawSetOutline(playerid, rndwstSelection_item_model[playerid][14], 0);
	PlayerTextDrawSetShadow(playerid, rndwstSelection_item_model[playerid][14], 0);
	PlayerTextDrawAlignment(playerid, rndwstSelection_item_model[playerid][14], 1);
	PlayerTextDrawColor(playerid, rndwstSelection_item_model[playerid][14], -1);
	PlayerTextDrawBackgroundColor(playerid, rndwstSelection_item_model[playerid][14], 125);
	PlayerTextDrawBoxColor(playerid, rndwstSelection_item_model[playerid][14], 255);
	PlayerTextDrawUseBox(playerid, rndwstSelection_item_model[playerid][14], 0);
	PlayerTextDrawSetProportional(playerid, rndwstSelection_item_model[playerid][14], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstSelection_item_model[playerid][14], 1);
	PlayerTextDrawSetPreviewModel(playerid, rndwstSelection_item_model[playerid][14], 19094);
	PlayerTextDrawSetPreviewRot(playerid, rndwstSelection_item_model[playerid][14], -10.000000, 0.000000, -20.000000, 1.000000);
	PlayerTextDrawSetPreviewVehCol(playerid, rndwstSelection_item_model[playerid][14], 1, 1);

	rndwstSelection_item_name[playerid][0] = CreatePlayerTextDraw(playerid, 262.000000, 167.000000, "Burger");
	PlayerTextDrawFont(playerid, rndwstSelection_item_name[playerid][0], 2);
	PlayerTextDrawLetterSize(playerid, rndwstSelection_item_name[playerid][0], 0.104166, 0.850000);
	PlayerTextDrawTextSize(playerid, rndwstSelection_item_name[playerid][0], 16.500000, 39.500000);
	PlayerTextDrawSetOutline(playerid, rndwstSelection_item_name[playerid][0], 0);
	PlayerTextDrawSetShadow(playerid, rndwstSelection_item_name[playerid][0], 0);
	PlayerTextDrawAlignment(playerid, rndwstSelection_item_name[playerid][0], 2);
	PlayerTextDrawColor(playerid, rndwstSelection_item_name[playerid][0], -1);
	PlayerTextDrawBackgroundColor(playerid, rndwstSelection_item_name[playerid][0], 255);
	PlayerTextDrawBoxColor(playerid, rndwstSelection_item_name[playerid][0], 200);
	PlayerTextDrawUseBox(playerid, rndwstSelection_item_name[playerid][0], 0);
	PlayerTextDrawSetProportional(playerid, rndwstSelection_item_name[playerid][0], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstSelection_item_name[playerid][0], 0);

	rndwstSelection_item_name[playerid][1] = CreatePlayerTextDraw(playerid, 317.000000, 167.000000, "Burger");
	PlayerTextDrawFont(playerid, rndwstSelection_item_name[playerid][1], 2);
	PlayerTextDrawLetterSize(playerid, rndwstSelection_item_name[playerid][1], 0.104166, 0.850000);
	PlayerTextDrawTextSize(playerid, rndwstSelection_item_name[playerid][1], 16.500000, 39.500000);
	PlayerTextDrawSetOutline(playerid, rndwstSelection_item_name[playerid][1], 0);
	PlayerTextDrawSetShadow(playerid, rndwstSelection_item_name[playerid][1], 0);
	PlayerTextDrawAlignment(playerid, rndwstSelection_item_name[playerid][1], 2);
	PlayerTextDrawColor(playerid, rndwstSelection_item_name[playerid][1], -1);
	PlayerTextDrawBackgroundColor(playerid, rndwstSelection_item_name[playerid][1], 255);
	PlayerTextDrawBoxColor(playerid, rndwstSelection_item_name[playerid][1], 200);
	PlayerTextDrawUseBox(playerid, rndwstSelection_item_name[playerid][1], 0);
	PlayerTextDrawSetProportional(playerid, rndwstSelection_item_name[playerid][1], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstSelection_item_name[playerid][1], 0);
	
	rndwstSelection_item_name[playerid][2] = CreatePlayerTextDraw(playerid, 372.000000, 167.000000, "Burger");
	PlayerTextDrawFont(playerid, rndwstSelection_item_name[playerid][2], 2);
	PlayerTextDrawLetterSize(playerid, rndwstSelection_item_name[playerid][2], 0.104166, 0.850000);
	PlayerTextDrawTextSize(playerid, rndwstSelection_item_name[playerid][2], 16.500000, 39.500000);
	PlayerTextDrawSetOutline(playerid, rndwstSelection_item_name[playerid][2], 0);
	PlayerTextDrawSetShadow(playerid, rndwstSelection_item_name[playerid][2], 0);
	PlayerTextDrawAlignment(playerid, rndwstSelection_item_name[playerid][2], 2);
	PlayerTextDrawColor(playerid, rndwstSelection_item_name[playerid][2], -1);
	PlayerTextDrawBackgroundColor(playerid, rndwstSelection_item_name[playerid][2], 255);
	PlayerTextDrawBoxColor(playerid, rndwstSelection_item_name[playerid][2], 200);
	PlayerTextDrawUseBox(playerid, rndwstSelection_item_name[playerid][2], 0);
	PlayerTextDrawSetProportional(playerid, rndwstSelection_item_name[playerid][2], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstSelection_item_name[playerid][2], 0);

	rndwstSelection_item_name[playerid][3] = CreatePlayerTextDraw(playerid, 427.000000, 167.000000, "Burger");
	PlayerTextDrawFont(playerid, rndwstSelection_item_name[playerid][3], 2);
	PlayerTextDrawLetterSize(playerid, rndwstSelection_item_name[playerid][3], 0.104166, 0.850000);
	PlayerTextDrawTextSize(playerid, rndwstSelection_item_name[playerid][3], 16.500000, 39.500000);
	PlayerTextDrawSetOutline(playerid, rndwstSelection_item_name[playerid][3], 0);
	PlayerTextDrawSetShadow(playerid, rndwstSelection_item_name[playerid][3], 0);
	PlayerTextDrawAlignment(playerid, rndwstSelection_item_name[playerid][3], 2);
	PlayerTextDrawColor(playerid, rndwstSelection_item_name[playerid][3], -1);
	PlayerTextDrawBackgroundColor(playerid, rndwstSelection_item_name[playerid][3], 255);
	PlayerTextDrawBoxColor(playerid, rndwstSelection_item_name[playerid][3], 200);
	PlayerTextDrawUseBox(playerid, rndwstSelection_item_name[playerid][3], 0);
	PlayerTextDrawSetProportional(playerid, rndwstSelection_item_name[playerid][3], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstSelection_item_name[playerid][3], 0);

	rndwstSelection_item_name[playerid][4] = CreatePlayerTextDraw(playerid, 482.000000, 167.000000, "Burger");
	PlayerTextDrawFont(playerid, rndwstSelection_item_name[playerid][4], 2);
	PlayerTextDrawLetterSize(playerid, rndwstSelection_item_name[playerid][4], 0.104166, 0.850000);
	PlayerTextDrawTextSize(playerid, rndwstSelection_item_name[playerid][4], 16.500000, 39.500000);
	PlayerTextDrawSetOutline(playerid, rndwstSelection_item_name[playerid][4], 0);
	PlayerTextDrawSetShadow(playerid, rndwstSelection_item_name[playerid][4], 0);
	PlayerTextDrawAlignment(playerid, rndwstSelection_item_name[playerid][4], 2);
	PlayerTextDrawColor(playerid, rndwstSelection_item_name[playerid][4], -1);
	PlayerTextDrawBackgroundColor(playerid, rndwstSelection_item_name[playerid][4], 255);
	PlayerTextDrawBoxColor(playerid, rndwstSelection_item_name[playerid][4], 200);
	PlayerTextDrawUseBox(playerid, rndwstSelection_item_name[playerid][4], 0);
	PlayerTextDrawSetProportional(playerid, rndwstSelection_item_name[playerid][4], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstSelection_item_name[playerid][4], 0);

	rndwstSelection_item_name[playerid][5] = CreatePlayerTextDraw(playerid, 262.000000, 222.000000, "Burger");
	PlayerTextDrawFont(playerid, rndwstSelection_item_name[playerid][5], 2);
	PlayerTextDrawLetterSize(playerid, rndwstSelection_item_name[playerid][5], 0.104166, 0.850000);
	PlayerTextDrawTextSize(playerid, rndwstSelection_item_name[playerid][5], 16.500000, 39.500000);
	PlayerTextDrawSetOutline(playerid, rndwstSelection_item_name[playerid][5], 0);
	PlayerTextDrawSetShadow(playerid, rndwstSelection_item_name[playerid][5], 0);
	PlayerTextDrawAlignment(playerid, rndwstSelection_item_name[playerid][5], 2);
	PlayerTextDrawColor(playerid, rndwstSelection_item_name[playerid][5], -1);
	PlayerTextDrawBackgroundColor(playerid, rndwstSelection_item_name[playerid][5], 255);
	PlayerTextDrawBoxColor(playerid, rndwstSelection_item_name[playerid][5], 200);
	PlayerTextDrawUseBox(playerid, rndwstSelection_item_name[playerid][5], 0);
	PlayerTextDrawSetProportional(playerid, rndwstSelection_item_name[playerid][5], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstSelection_item_name[playerid][5], 0);

	rndwstSelection_item_name[playerid][6] = CreatePlayerTextDraw(playerid, 317.000000, 222.000000, "Burger");
	PlayerTextDrawFont(playerid, rndwstSelection_item_name[playerid][6], 2);
	PlayerTextDrawLetterSize(playerid, rndwstSelection_item_name[playerid][6], 0.104166, 0.850000);
	PlayerTextDrawTextSize(playerid, rndwstSelection_item_name[playerid][6], 16.500000, 39.500000);
	PlayerTextDrawSetOutline(playerid, rndwstSelection_item_name[playerid][6], 0);
	PlayerTextDrawSetShadow(playerid, rndwstSelection_item_name[playerid][6], 0);
	PlayerTextDrawAlignment(playerid, rndwstSelection_item_name[playerid][6], 2);
	PlayerTextDrawColor(playerid, rndwstSelection_item_name[playerid][6], -1);
	PlayerTextDrawBackgroundColor(playerid, rndwstSelection_item_name[playerid][6], 255);
	PlayerTextDrawBoxColor(playerid, rndwstSelection_item_name[playerid][6], 200);
	PlayerTextDrawUseBox(playerid, rndwstSelection_item_name[playerid][6], 0);
	PlayerTextDrawSetProportional(playerid, rndwstSelection_item_name[playerid][6], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstSelection_item_name[playerid][6], 0);

	rndwstSelection_item_name[playerid][7] = CreatePlayerTextDraw(playerid, 372.000000, 222.000000, "Burger");
	PlayerTextDrawFont(playerid, rndwstSelection_item_name[playerid][7], 2);
	PlayerTextDrawLetterSize(playerid, rndwstSelection_item_name[playerid][7], 0.104166, 0.850000);
	PlayerTextDrawTextSize(playerid, rndwstSelection_item_name[playerid][7], 16.500000, 39.500000);
	PlayerTextDrawSetOutline(playerid, rndwstSelection_item_name[playerid][7], 0);
	PlayerTextDrawSetShadow(playerid, rndwstSelection_item_name[playerid][7], 0);
	PlayerTextDrawAlignment(playerid, rndwstSelection_item_name[playerid][7], 2);
	PlayerTextDrawColor(playerid, rndwstSelection_item_name[playerid][7], -1);
	PlayerTextDrawBackgroundColor(playerid, rndwstSelection_item_name[playerid][7], 255);
	PlayerTextDrawBoxColor(playerid, rndwstSelection_item_name[playerid][7], 200);
	PlayerTextDrawUseBox(playerid, rndwstSelection_item_name[playerid][7], 0);
	PlayerTextDrawSetProportional(playerid, rndwstSelection_item_name[playerid][7], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstSelection_item_name[playerid][7], 0);

	rndwstSelection_item_name[playerid][8] = CreatePlayerTextDraw(playerid, 427.000000, 222.000000, "Burger");
	PlayerTextDrawFont(playerid, rndwstSelection_item_name[playerid][8], 2);
	PlayerTextDrawLetterSize(playerid, rndwstSelection_item_name[playerid][8], 0.104166, 0.850000);
	PlayerTextDrawTextSize(playerid, rndwstSelection_item_name[playerid][8], 16.500000, 39.500000);
	PlayerTextDrawSetOutline(playerid, rndwstSelection_item_name[playerid][8], 0);
	PlayerTextDrawSetShadow(playerid, rndwstSelection_item_name[playerid][8], 0);
	PlayerTextDrawAlignment(playerid, rndwstSelection_item_name[playerid][8], 2);
	PlayerTextDrawColor(playerid, rndwstSelection_item_name[playerid][8], -1);
	PlayerTextDrawBackgroundColor(playerid, rndwstSelection_item_name[playerid][8], 255);
	PlayerTextDrawBoxColor(playerid, rndwstSelection_item_name[playerid][8], 200);
	PlayerTextDrawUseBox(playerid, rndwstSelection_item_name[playerid][8], 0);
	PlayerTextDrawSetProportional(playerid, rndwstSelection_item_name[playerid][8], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstSelection_item_name[playerid][8], 0);

	rndwstSelection_item_name[playerid][9] = CreatePlayerTextDraw(playerid, 482.000000, 222.000000, "Burger");
	PlayerTextDrawFont(playerid, rndwstSelection_item_name[playerid][9], 2);
	PlayerTextDrawLetterSize(playerid, rndwstSelection_item_name[playerid][9], 0.104166, 0.850000);
	PlayerTextDrawTextSize(playerid, rndwstSelection_item_name[playerid][9], 16.500000, 39.500000);
	PlayerTextDrawSetOutline(playerid, rndwstSelection_item_name[playerid][9], 0);
	PlayerTextDrawSetShadow(playerid, rndwstSelection_item_name[playerid][9], 0);
	PlayerTextDrawAlignment(playerid, rndwstSelection_item_name[playerid][9], 2);
	PlayerTextDrawColor(playerid, rndwstSelection_item_name[playerid][9], -1);
	PlayerTextDrawBackgroundColor(playerid, rndwstSelection_item_name[playerid][9], 255);
	PlayerTextDrawBoxColor(playerid, rndwstSelection_item_name[playerid][9], 200);
	PlayerTextDrawUseBox(playerid, rndwstSelection_item_name[playerid][9], 0);
	PlayerTextDrawSetProportional(playerid, rndwstSelection_item_name[playerid][9], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstSelection_item_name[playerid][9], 0);

	rndwstSelection_item_name[playerid][10] = CreatePlayerTextDraw(playerid, 262.000000, 277.000000, "Burger");
	PlayerTextDrawFont(playerid, rndwstSelection_item_name[playerid][10], 2);
	PlayerTextDrawLetterSize(playerid, rndwstSelection_item_name[playerid][10], 0.104166, 0.850000);
	PlayerTextDrawTextSize(playerid, rndwstSelection_item_name[playerid][10], 16.500000, 39.500000);
	PlayerTextDrawSetOutline(playerid, rndwstSelection_item_name[playerid][10], 0);
	PlayerTextDrawSetShadow(playerid, rndwstSelection_item_name[playerid][10], 0);
	PlayerTextDrawAlignment(playerid, rndwstSelection_item_name[playerid][10], 2);
	PlayerTextDrawColor(playerid, rndwstSelection_item_name[playerid][10], -1);
	PlayerTextDrawBackgroundColor(playerid, rndwstSelection_item_name[playerid][10], 255);
	PlayerTextDrawBoxColor(playerid, rndwstSelection_item_name[playerid][10], 200);
	PlayerTextDrawUseBox(playerid, rndwstSelection_item_name[playerid][10], 0);
	PlayerTextDrawSetProportional(playerid, rndwstSelection_item_name[playerid][10], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstSelection_item_name[playerid][10], 0);

	rndwstSelection_item_name[playerid][11] = CreatePlayerTextDraw(playerid, 317.000000, 277.000000, "Burger");
	PlayerTextDrawFont(playerid, rndwstSelection_item_name[playerid][11], 2);
	PlayerTextDrawLetterSize(playerid, rndwstSelection_item_name[playerid][11], 0.104166, 0.850000);
	PlayerTextDrawTextSize(playerid, rndwstSelection_item_name[playerid][11], 16.500000, 39.500000);
	PlayerTextDrawSetOutline(playerid, rndwstSelection_item_name[playerid][11], 0);
	PlayerTextDrawSetShadow(playerid, rndwstSelection_item_name[playerid][11], 0);
	PlayerTextDrawAlignment(playerid, rndwstSelection_item_name[playerid][11], 2);
	PlayerTextDrawColor(playerid, rndwstSelection_item_name[playerid][11], -1);
	PlayerTextDrawBackgroundColor(playerid, rndwstSelection_item_name[playerid][11], 255);
	PlayerTextDrawBoxColor(playerid, rndwstSelection_item_name[playerid][11], 200);
	PlayerTextDrawUseBox(playerid, rndwstSelection_item_name[playerid][11], 0);
	PlayerTextDrawSetProportional(playerid, rndwstSelection_item_name[playerid][11], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstSelection_item_name[playerid][11], 0);

	rndwstSelection_item_name[playerid][12] = CreatePlayerTextDraw(playerid, 372.000000, 277.000000, "Burger");
	PlayerTextDrawFont(playerid, rndwstSelection_item_name[playerid][12], 2);
	PlayerTextDrawLetterSize(playerid, rndwstSelection_item_name[playerid][12], 0.104166, 0.850000);
	PlayerTextDrawTextSize(playerid, rndwstSelection_item_name[playerid][12], 16.500000, 39.500000);
	PlayerTextDrawSetOutline(playerid, rndwstSelection_item_name[playerid][12], 0);
	PlayerTextDrawSetShadow(playerid, rndwstSelection_item_name[playerid][12], 0);
	PlayerTextDrawAlignment(playerid, rndwstSelection_item_name[playerid][12], 2);
	PlayerTextDrawColor(playerid, rndwstSelection_item_name[playerid][12], -1);
	PlayerTextDrawBackgroundColor(playerid, rndwstSelection_item_name[playerid][12], 255);
	PlayerTextDrawBoxColor(playerid, rndwstSelection_item_name[playerid][12], 200);
	PlayerTextDrawUseBox(playerid, rndwstSelection_item_name[playerid][12], 0);
	PlayerTextDrawSetProportional(playerid, rndwstSelection_item_name[playerid][12], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstSelection_item_name[playerid][12], 0);

	rndwstSelection_item_name[playerid][13] = CreatePlayerTextDraw(playerid, 427.000000, 277.000000, "Burger");
	PlayerTextDrawFont(playerid, rndwstSelection_item_name[playerid][13], 2);
	PlayerTextDrawLetterSize(playerid, rndwstSelection_item_name[playerid][13], 0.104166, 0.850000);
	PlayerTextDrawTextSize(playerid, rndwstSelection_item_name[playerid][13], 16.500000, 39.500000);
	PlayerTextDrawSetOutline(playerid, rndwstSelection_item_name[playerid][13], 0);
	PlayerTextDrawSetShadow(playerid, rndwstSelection_item_name[playerid][13], 0);
	PlayerTextDrawAlignment(playerid, rndwstSelection_item_name[playerid][13], 2);
	PlayerTextDrawColor(playerid, rndwstSelection_item_name[playerid][13], -1);
	PlayerTextDrawBackgroundColor(playerid, rndwstSelection_item_name[playerid][13], 255);
	PlayerTextDrawBoxColor(playerid, rndwstSelection_item_name[playerid][13], 200);
	PlayerTextDrawUseBox(playerid, rndwstSelection_item_name[playerid][13], 0);
	PlayerTextDrawSetProportional(playerid, rndwstSelection_item_name[playerid][13], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstSelection_item_name[playerid][13], 0);

	rndwstSelection_item_name[playerid][14] = CreatePlayerTextDraw(playerid, 482.000000, 277.000000, "Burger");
	PlayerTextDrawFont(playerid, rndwstSelection_item_name[playerid][14], 2);
	PlayerTextDrawLetterSize(playerid, rndwstSelection_item_name[playerid][14], 0.104166, 0.850000);
	PlayerTextDrawTextSize(playerid, rndwstSelection_item_name[playerid][14], 16.500000, 39.500000);
	PlayerTextDrawSetOutline(playerid, rndwstSelection_item_name[playerid][14], 0);
	PlayerTextDrawSetShadow(playerid, rndwstSelection_item_name[playerid][14], 0);
	PlayerTextDrawAlignment(playerid, rndwstSelection_item_name[playerid][14], 2);
	PlayerTextDrawColor(playerid, rndwstSelection_item_name[playerid][14], -1);
	PlayerTextDrawBackgroundColor(playerid, rndwstSelection_item_name[playerid][14], 255);
	PlayerTextDrawBoxColor(playerid, rndwstSelection_item_name[playerid][14], 200);
	PlayerTextDrawUseBox(playerid, rndwstSelection_item_name[playerid][14], 0);
	PlayerTextDrawSetProportional(playerid, rndwstSelection_item_name[playerid][14], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstSelection_item_name[playerid][14], 0);

	rndwstSelection_item_amount[playerid][0] = CreatePlayerTextDraw(playerid, 262.000000, 203.000000, "20");
	PlayerTextDrawFont(playerid, rndwstSelection_item_amount[playerid][0], 2);
	PlayerTextDrawLetterSize(playerid, rndwstSelection_item_amount[playerid][0], 0.104166, 0.850000);
	PlayerTextDrawTextSize(playerid, rndwstSelection_item_amount[playerid][0], 16.500000, 39.500000);
	PlayerTextDrawSetOutline(playerid, rndwstSelection_item_amount[playerid][0], 0);
	PlayerTextDrawSetShadow(playerid, rndwstSelection_item_amount[playerid][0], 0);
	PlayerTextDrawAlignment(playerid, rndwstSelection_item_amount[playerid][0], 2);
	PlayerTextDrawColor(playerid, rndwstSelection_item_amount[playerid][0], -1);
	PlayerTextDrawBackgroundColor(playerid, rndwstSelection_item_amount[playerid][0], 255);
	PlayerTextDrawBoxColor(playerid, rndwstSelection_item_amount[playerid][0], 200);
	PlayerTextDrawUseBox(playerid, rndwstSelection_item_amount[playerid][0], 0);
	PlayerTextDrawSetProportional(playerid, rndwstSelection_item_amount[playerid][0], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstSelection_item_amount[playerid][0], 0);

	rndwstSelection_item_amount[playerid][1] = CreatePlayerTextDraw(playerid, 317.000000, 203.000000, "20");
	PlayerTextDrawFont(playerid, rndwstSelection_item_amount[playerid][1], 2);
	PlayerTextDrawLetterSize(playerid, rndwstSelection_item_amount[playerid][1], 0.104166, 0.850000);
	PlayerTextDrawTextSize(playerid, rndwstSelection_item_amount[playerid][1], 16.500000, 39.500000);
	PlayerTextDrawSetOutline(playerid, rndwstSelection_item_amount[playerid][1], 0);
	PlayerTextDrawSetShadow(playerid, rndwstSelection_item_amount[playerid][1], 0);
	PlayerTextDrawAlignment(playerid, rndwstSelection_item_amount[playerid][1], 2);
	PlayerTextDrawColor(playerid, rndwstSelection_item_amount[playerid][1], -1);
	PlayerTextDrawBackgroundColor(playerid, rndwstSelection_item_amount[playerid][1], 255);
	PlayerTextDrawBoxColor(playerid, rndwstSelection_item_amount[playerid][1], 200);
	PlayerTextDrawUseBox(playerid, rndwstSelection_item_amount[playerid][1], 0);
	PlayerTextDrawSetProportional(playerid, rndwstSelection_item_amount[playerid][1], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstSelection_item_amount[playerid][1], 0);

	rndwstSelection_item_amount[playerid][2] = CreatePlayerTextDraw(playerid, 372.000000, 203.000000, "20");
	PlayerTextDrawFont(playerid, rndwstSelection_item_amount[playerid][2], 2);
	PlayerTextDrawLetterSize(playerid, rndwstSelection_item_amount[playerid][2], 0.104166, 0.850000);
	PlayerTextDrawTextSize(playerid, rndwstSelection_item_amount[playerid][2], 16.500000, 39.500000);
	PlayerTextDrawSetOutline(playerid, rndwstSelection_item_amount[playerid][2], 0);
	PlayerTextDrawSetShadow(playerid, rndwstSelection_item_amount[playerid][2], 0);
	PlayerTextDrawAlignment(playerid, rndwstSelection_item_amount[playerid][2], 2);
	PlayerTextDrawColor(playerid, rndwstSelection_item_amount[playerid][2], -1);
	PlayerTextDrawBackgroundColor(playerid, rndwstSelection_item_amount[playerid][2], 255);
	PlayerTextDrawBoxColor(playerid, rndwstSelection_item_amount[playerid][2], 200);
	PlayerTextDrawUseBox(playerid, rndwstSelection_item_amount[playerid][2], 0);
	PlayerTextDrawSetProportional(playerid, rndwstSelection_item_amount[playerid][2], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstSelection_item_amount[playerid][2], 0);

	rndwstSelection_item_amount[playerid][3] = CreatePlayerTextDraw(playerid, 427.000000, 203.000000, "20");
	PlayerTextDrawFont(playerid, rndwstSelection_item_amount[playerid][3], 2);
	PlayerTextDrawLetterSize(playerid, rndwstSelection_item_amount[playerid][3], 0.104166, 0.850000);
	PlayerTextDrawTextSize(playerid, rndwstSelection_item_amount[playerid][3], 16.500000, 39.500000);
	PlayerTextDrawSetOutline(playerid, rndwstSelection_item_amount[playerid][3], 0);
	PlayerTextDrawSetShadow(playerid, rndwstSelection_item_amount[playerid][3], 0);
	PlayerTextDrawAlignment(playerid, rndwstSelection_item_amount[playerid][3], 2);
	PlayerTextDrawColor(playerid, rndwstSelection_item_amount[playerid][3], -1);
	PlayerTextDrawBackgroundColor(playerid, rndwstSelection_item_amount[playerid][3], 255);
	PlayerTextDrawBoxColor(playerid, rndwstSelection_item_amount[playerid][3], 200);
	PlayerTextDrawUseBox(playerid, rndwstSelection_item_amount[playerid][3], 0);
	PlayerTextDrawSetProportional(playerid, rndwstSelection_item_amount[playerid][3], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstSelection_item_amount[playerid][3], 0);

	rndwstSelection_item_amount[playerid][4] = CreatePlayerTextDraw(playerid, 482.000000, 203.000000, "20");
	PlayerTextDrawFont(playerid, rndwstSelection_item_amount[playerid][4], 2);
	PlayerTextDrawLetterSize(playerid, rndwstSelection_item_amount[playerid][4], 0.104166, 0.850000);
	PlayerTextDrawTextSize(playerid, rndwstSelection_item_amount[playerid][4], 16.500000, 39.500000);
	PlayerTextDrawSetOutline(playerid, rndwstSelection_item_amount[playerid][4], 0);
	PlayerTextDrawSetShadow(playerid, rndwstSelection_item_amount[playerid][4], 0);
	PlayerTextDrawAlignment(playerid, rndwstSelection_item_amount[playerid][4], 2);
	PlayerTextDrawColor(playerid, rndwstSelection_item_amount[playerid][4], -1);
	PlayerTextDrawBackgroundColor(playerid, rndwstSelection_item_amount[playerid][4], 255);
	PlayerTextDrawBoxColor(playerid, rndwstSelection_item_amount[playerid][4], 200);
	PlayerTextDrawUseBox(playerid, rndwstSelection_item_amount[playerid][4], 0);
	PlayerTextDrawSetProportional(playerid, rndwstSelection_item_amount[playerid][4], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstSelection_item_amount[playerid][4], 0);

	rndwstSelection_item_amount[playerid][5] = CreatePlayerTextDraw(playerid, 262.000000, 258.000000, "20");
	PlayerTextDrawFont(playerid, rndwstSelection_item_amount[playerid][5], 2);
	PlayerTextDrawLetterSize(playerid, rndwstSelection_item_amount[playerid][5], 0.104166, 0.850000);
	PlayerTextDrawTextSize(playerid, rndwstSelection_item_amount[playerid][5], 16.500000, 39.500000);
	PlayerTextDrawSetOutline(playerid, rndwstSelection_item_amount[playerid][5], 0);
	PlayerTextDrawSetShadow(playerid, rndwstSelection_item_amount[playerid][5], 0);
	PlayerTextDrawAlignment(playerid, rndwstSelection_item_amount[playerid][5], 2);
	PlayerTextDrawColor(playerid, rndwstSelection_item_amount[playerid][5], -1);
	PlayerTextDrawBackgroundColor(playerid, rndwstSelection_item_amount[playerid][5], 255);
	PlayerTextDrawBoxColor(playerid, rndwstSelection_item_amount[playerid][5], 200);
	PlayerTextDrawUseBox(playerid, rndwstSelection_item_amount[playerid][5], 0);
	PlayerTextDrawSetProportional(playerid, rndwstSelection_item_amount[playerid][5], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstSelection_item_amount[playerid][5], 0);

	rndwstSelection_item_amount[playerid][6] = CreatePlayerTextDraw(playerid, 317.000000, 258.000000, "20");
	PlayerTextDrawFont(playerid, rndwstSelection_item_amount[playerid][6], 2);
	PlayerTextDrawLetterSize(playerid, rndwstSelection_item_amount[playerid][6], 0.104166, 0.850000);
	PlayerTextDrawTextSize(playerid, rndwstSelection_item_amount[playerid][6], 16.500000, 39.500000);
	PlayerTextDrawSetOutline(playerid, rndwstSelection_item_amount[playerid][6], 0);
	PlayerTextDrawSetShadow(playerid, rndwstSelection_item_amount[playerid][6], 0);
	PlayerTextDrawAlignment(playerid, rndwstSelection_item_amount[playerid][6], 2);
	PlayerTextDrawColor(playerid, rndwstSelection_item_amount[playerid][6], -1);
	PlayerTextDrawBackgroundColor(playerid, rndwstSelection_item_amount[playerid][6], 255);
	PlayerTextDrawBoxColor(playerid, rndwstSelection_item_amount[playerid][6], 200);
	PlayerTextDrawUseBox(playerid, rndwstSelection_item_amount[playerid][6], 0);
	PlayerTextDrawSetProportional(playerid, rndwstSelection_item_amount[playerid][6], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstSelection_item_amount[playerid][6], 0);

	rndwstSelection_item_amount[playerid][7] = CreatePlayerTextDraw(playerid, 372.000000, 258.000000, "20");
	PlayerTextDrawFont(playerid, rndwstSelection_item_amount[playerid][7], 2);
	PlayerTextDrawLetterSize(playerid, rndwstSelection_item_amount[playerid][7], 0.104166, 0.850000);
	PlayerTextDrawTextSize(playerid, rndwstSelection_item_amount[playerid][7], 16.500000, 39.500000);
	PlayerTextDrawSetOutline(playerid, rndwstSelection_item_amount[playerid][7], 0);
	PlayerTextDrawSetShadow(playerid, rndwstSelection_item_amount[playerid][7], 0);
	PlayerTextDrawAlignment(playerid, rndwstSelection_item_amount[playerid][7], 2);
	PlayerTextDrawColor(playerid, rndwstSelection_item_amount[playerid][7], -1);
	PlayerTextDrawBackgroundColor(playerid, rndwstSelection_item_amount[playerid][7], 255);
	PlayerTextDrawBoxColor(playerid, rndwstSelection_item_amount[playerid][7], 200);
	PlayerTextDrawUseBox(playerid, rndwstSelection_item_amount[playerid][7], 0);
	PlayerTextDrawSetProportional(playerid, rndwstSelection_item_amount[playerid][7], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstSelection_item_amount[playerid][7], 0);

	rndwstSelection_item_amount[playerid][8] = CreatePlayerTextDraw(playerid, 427.000000, 258.000000, "20");
	PlayerTextDrawFont(playerid, rndwstSelection_item_amount[playerid][8], 2);
	PlayerTextDrawLetterSize(playerid, rndwstSelection_item_amount[playerid][8], 0.104166, 0.850000);
	PlayerTextDrawTextSize(playerid, rndwstSelection_item_amount[playerid][8], 16.500000, 39.500000);
	PlayerTextDrawSetOutline(playerid, rndwstSelection_item_amount[playerid][8], 0);
	PlayerTextDrawSetShadow(playerid, rndwstSelection_item_amount[playerid][8], 0);
	PlayerTextDrawAlignment(playerid, rndwstSelection_item_amount[playerid][8], 2);
	PlayerTextDrawColor(playerid, rndwstSelection_item_amount[playerid][8], -1);
	PlayerTextDrawBackgroundColor(playerid, rndwstSelection_item_amount[playerid][8], 255);
	PlayerTextDrawBoxColor(playerid, rndwstSelection_item_amount[playerid][8], 200);
	PlayerTextDrawUseBox(playerid, rndwstSelection_item_amount[playerid][8], 0);
	PlayerTextDrawSetProportional(playerid, rndwstSelection_item_amount[playerid][8], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstSelection_item_amount[playerid][8], 0);

	rndwstSelection_item_amount[playerid][9] = CreatePlayerTextDraw(playerid, 482.000000, 258.000000, "20");
	PlayerTextDrawFont(playerid, rndwstSelection_item_amount[playerid][9], 2);
	PlayerTextDrawLetterSize(playerid, rndwstSelection_item_amount[playerid][9], 0.104166, 0.850000);
	PlayerTextDrawTextSize(playerid, rndwstSelection_item_amount[playerid][9], 16.500000, 39.500000);
	PlayerTextDrawSetOutline(playerid, rndwstSelection_item_amount[playerid][9], 0);
	PlayerTextDrawSetShadow(playerid, rndwstSelection_item_amount[playerid][9], 0);
	PlayerTextDrawAlignment(playerid, rndwstSelection_item_amount[playerid][9], 2);
	PlayerTextDrawColor(playerid, rndwstSelection_item_amount[playerid][9], -1);
	PlayerTextDrawBackgroundColor(playerid, rndwstSelection_item_amount[playerid][9], 255);
	PlayerTextDrawBoxColor(playerid, rndwstSelection_item_amount[playerid][9], 200);
	PlayerTextDrawUseBox(playerid, rndwstSelection_item_amount[playerid][9], 0);
	PlayerTextDrawSetProportional(playerid, rndwstSelection_item_amount[playerid][9], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstSelection_item_amount[playerid][9], 0);

	rndwstSelection_item_amount[playerid][10] = CreatePlayerTextDraw(playerid, 262.000000, 313.000000, "20");
	PlayerTextDrawFont(playerid, rndwstSelection_item_amount[playerid][10], 2);
	PlayerTextDrawLetterSize(playerid, rndwstSelection_item_amount[playerid][10], 0.104166, 0.850000);
	PlayerTextDrawTextSize(playerid, rndwstSelection_item_amount[playerid][10], 16.500000, 39.500000);
	PlayerTextDrawSetOutline(playerid, rndwstSelection_item_amount[playerid][10], 0);
	PlayerTextDrawSetShadow(playerid, rndwstSelection_item_amount[playerid][10], 0);
	PlayerTextDrawAlignment(playerid, rndwstSelection_item_amount[playerid][10], 2);
	PlayerTextDrawColor(playerid, rndwstSelection_item_amount[playerid][10], -1);
	PlayerTextDrawBackgroundColor(playerid, rndwstSelection_item_amount[playerid][10], 255);
	PlayerTextDrawBoxColor(playerid, rndwstSelection_item_amount[playerid][10], 200);
	PlayerTextDrawUseBox(playerid, rndwstSelection_item_amount[playerid][10], 0);
	PlayerTextDrawSetProportional(playerid, rndwstSelection_item_amount[playerid][10], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstSelection_item_amount[playerid][10], 0);
	
	rndwstSelection_item_amount[playerid][11] = CreatePlayerTextDraw(playerid, 317.000000, 313.000000, "20");
	PlayerTextDrawFont(playerid, rndwstSelection_item_amount[playerid][11], 2);
	PlayerTextDrawLetterSize(playerid, rndwstSelection_item_amount[playerid][11], 0.104166, 0.850000);
	PlayerTextDrawTextSize(playerid, rndwstSelection_item_amount[playerid][11], 16.500000, 39.500000);
	PlayerTextDrawSetOutline(playerid, rndwstSelection_item_amount[playerid][11], 0);
	PlayerTextDrawSetShadow(playerid, rndwstSelection_item_amount[playerid][11], 0);
	PlayerTextDrawAlignment(playerid, rndwstSelection_item_amount[playerid][11], 2);
	PlayerTextDrawColor(playerid, rndwstSelection_item_amount[playerid][11], -1);
	PlayerTextDrawBackgroundColor(playerid, rndwstSelection_item_amount[playerid][11], 255);
	PlayerTextDrawBoxColor(playerid, rndwstSelection_item_amount[playerid][11], 200);
	PlayerTextDrawUseBox(playerid, rndwstSelection_item_amount[playerid][11], 0);
	PlayerTextDrawSetProportional(playerid, rndwstSelection_item_amount[playerid][11], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstSelection_item_amount[playerid][11], 0);

	rndwstSelection_item_amount[playerid][12] = CreatePlayerTextDraw(playerid, 372.000000, 313.000000, "20");
	PlayerTextDrawFont(playerid, rndwstSelection_item_amount[playerid][12], 2);
	PlayerTextDrawLetterSize(playerid, rndwstSelection_item_amount[playerid][12], 0.104166, 0.850000);
	PlayerTextDrawTextSize(playerid, rndwstSelection_item_amount[playerid][12], 16.500000, 39.500000);
	PlayerTextDrawSetOutline(playerid, rndwstSelection_item_amount[playerid][12], 0);
	PlayerTextDrawSetShadow(playerid, rndwstSelection_item_amount[playerid][12], 0);
	PlayerTextDrawAlignment(playerid, rndwstSelection_item_amount[playerid][12], 2);
	PlayerTextDrawColor(playerid, rndwstSelection_item_amount[playerid][12], -1);
	PlayerTextDrawBackgroundColor(playerid, rndwstSelection_item_amount[playerid][12], 255);
	PlayerTextDrawBoxColor(playerid, rndwstSelection_item_amount[playerid][12], 200);
	PlayerTextDrawUseBox(playerid, rndwstSelection_item_amount[playerid][12], 0);
	PlayerTextDrawSetProportional(playerid, rndwstSelection_item_amount[playerid][12], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstSelection_item_amount[playerid][12], 0);

	rndwstSelection_item_amount[playerid][13] = CreatePlayerTextDraw(playerid, 427.000000, 313.000000, "20");
	PlayerTextDrawFont(playerid, rndwstSelection_item_amount[playerid][13], 2);
	PlayerTextDrawLetterSize(playerid, rndwstSelection_item_amount[playerid][13], 0.104166, 0.850000);
	PlayerTextDrawTextSize(playerid, rndwstSelection_item_amount[playerid][13], 16.500000, 39.500000);
	PlayerTextDrawSetOutline(playerid, rndwstSelection_item_amount[playerid][13], 0);
	PlayerTextDrawSetShadow(playerid, rndwstSelection_item_amount[playerid][13], 0);
	PlayerTextDrawAlignment(playerid, rndwstSelection_item_amount[playerid][13], 2);
	PlayerTextDrawColor(playerid, rndwstSelection_item_amount[playerid][13], -1);
	PlayerTextDrawBackgroundColor(playerid, rndwstSelection_item_amount[playerid][13], 255);
	PlayerTextDrawBoxColor(playerid, rndwstSelection_item_amount[playerid][13], 200);
	PlayerTextDrawUseBox(playerid, rndwstSelection_item_amount[playerid][13], 0);
	PlayerTextDrawSetProportional(playerid, rndwstSelection_item_amount[playerid][13], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstSelection_item_amount[playerid][13], 0);

	rndwstSelection_item_amount[playerid][14] = CreatePlayerTextDraw(playerid, 482.000000, 313.000000, "20");
	PlayerTextDrawFont(playerid, rndwstSelection_item_amount[playerid][14], 2);
	PlayerTextDrawLetterSize(playerid, rndwstSelection_item_amount[playerid][14], 0.104166, 0.850000);
	PlayerTextDrawTextSize(playerid, rndwstSelection_item_amount[playerid][14], 16.500000, 39.500000);
	PlayerTextDrawSetOutline(playerid, rndwstSelection_item_amount[playerid][14], 0);
	PlayerTextDrawSetShadow(playerid, rndwstSelection_item_amount[playerid][14], 0);
	PlayerTextDrawAlignment(playerid, rndwstSelection_item_amount[playerid][14], 2);
	PlayerTextDrawColor(playerid, rndwstSelection_item_amount[playerid][14], -1);
	PlayerTextDrawBackgroundColor(playerid, rndwstSelection_item_amount[playerid][14], 255);
	PlayerTextDrawBoxColor(playerid, rndwstSelection_item_amount[playerid][14], 200);
	PlayerTextDrawUseBox(playerid, rndwstSelection_item_amount[playerid][14], 0);
	PlayerTextDrawSetProportional(playerid, rndwstSelection_item_amount[playerid][14], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstSelection_item_amount[playerid][14], 0);

	rndwstSelection_player_loaded[playerid] = true;
	return 1;
}

#if defined _ALS_OnPlayerClickTextDraw
	#undef OnPlayerClickTextDraw
#else
	#define _ALS_OnPlayerClickTextDraw
#endif

#if defined _ALS_OnPlayerClickPlayerTD
	#undef OnPlayerClickPlayerTextDraw
#else
	#define _ALS_OnPlayerClickPlayerTD
#endif

#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif

#if defined _ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
	#define _ALS_OnPlayerDisconnect
#endif


#define OnPlayerClickTextDraw RN_OnPlayerClickTextDraw
#define OnPlayerClickPlayerTextDraw RN_OnPlayerClickPlayerTextDraw
#define OnPlayerConnect RN_OnPlayerConnect
#define OnPlayerDisconnect RN_OnPlayerDisconnect

forward RN_OnPlayerClickTextDraw(playerid, Text:clickedid);
forward RN_OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid);
forward RN_OnPlayerConnect(playerid);
forward RN_OnPlayerDisconnect(playerid, reason);