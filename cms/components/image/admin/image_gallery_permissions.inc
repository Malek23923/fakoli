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
 
Fakoli::using("image");
Fakoli::usingFeature("auto_form");

require_once "include/image_gallery_tabs.inc";

$menu_item = "Images";

$gallery_id = checkNumeric($_GET["gallery_id"]);

if (!$gallery_id) redirect("/admin/image_galleries");

$gallery = new ImageGallery($gallery_id);

$gallery->filter = new InclusionFilter("gallery_id", "read_access", "write_access");

$tabs = imageGalleryTabs($gallery_id);

$form = new AutoForm($gallery);
$form->hide("owner_id");
$readSelect = new CheckListFieldRenderer($form, "read_access", "Read Access", SiteRole::getRolesArray());
$readSelect->setSize(350, 150, 10);
$writeSelect = new CheckListFieldRenderer($form, "write_access", "Write Access", SiteRole::getRolesArray());
$writeSelect->setSize(350, 150, 10);

if ($method == "POST")
{
	if ($form->save())
	{
		$tabs->next();
	}
}


$script .= $form->writeScript();

$tabs->writeHTML();
?>
<div id="tab_border" style="margin-right: -2px;">
<?

$form->drawForm();
?>
</div>