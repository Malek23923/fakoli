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

Fakoli::using("online_help");
Fakoli::usingFeature("auto_form", "tab_bar");

$menu_item = "Online Help";
$help_book_id = checkNumeric($_GET["help_book_id"]);

$helpBook = new HelpBook();

if ($help_book_id)
{
	$helpBook->load($help_book_id);
	$title = "Edit Help Book";
}
else
{
	$title = "New Help Book";
}

$tabs = OnlineHelpManager::getAdminTabs($help_book_id);

$form = new AutoForm($helpBook);
$form->alias("remote", "Remote Help Book");
$form->alias("url", "Help Book URL");

$form->required("title", "identifier");
$form->requiredIfChecked("url", "remote");

$form->allowDelete = true;

if ($method == "POST")
{
	if ($form->save())
	{
		if ($helpBook->remote) redirect("help_books");
	}
}

$script .= $form->writeScript();

$tabs->writeHTML();
?>
<div style="clear:left; border: solid 1px #000; width: 100%; margin: 0; padding: 4px;">
<?
$form->drawForm();
?>
</div>