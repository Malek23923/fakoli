<?php
/**************************************************************

 Copyright (c) 2010 Sonjara, Inc

 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use,
 copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following
 conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 Except as contained in this notice, the name(s) of the above 
 copyright holders shall not be used in advertising or otherwise 
 to promote the sale, use or other dealings in this Software 
 without prior written authorization.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.

*****************************************************************/

Fakoli::using("calendar");
Fakoli::usingFile("framework/data_view.inc");

$title = "Calendars";

$calendars = query(Calendar, "ORDER BY name");

$menu_item = "Calendars";

$table = new DataListView($calendars, "calendars");
$table->column("Calendar", "<a href='/admin/calendar_form?calendar_id={calendar_id}'>{name}</a>", true)
	  ->column("Identifier", "{identifier}", true);
	  
$table->sortable = true;
$table->filter = true;
$table->pageSize = 20;
$table->emptyMessage = "No Calendars defined";

$script .= $table->writeScript();

$table->drawView();
?>
<br/>
<input type="button" class="button" value="  Add a new Calendar  " onclick="go('/admin/calendar_form');">