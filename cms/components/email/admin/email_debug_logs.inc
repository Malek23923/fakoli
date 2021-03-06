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

Fakoli::using("email");
Fakoli::usingFeature("data_view");

$menu_item = "Email Debug Logs";

$title = "Email Debug Logs";

$logs = EmailManager::scanDebugLogs();

$table = new DataListView($logs, "emailDebugLogs");
$table->column("Recipient", "<a href=\"/action/email/download_email_log?filename={filename}\">{filename}</a>", true)
		->column("Last Modified", "{date:Y-m-d g:ia}", true)
		;
			  	
$table->emptyMessage = "There are no email debug logs.";
$table->mode = "tree";
$table->cssStyle = "width: 80%";
$table->filter = true;
$table->sortable = true;

$script .= $table->writeScript();

echo "<p>All emails sent to a recipient are stored in one file with the recipient as the filename.</p>\n";
$table->drawView();

?>