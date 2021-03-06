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

/**************************************************************
 *
 * Description: Update/edit site map page for the
 * site hierarchy chart/tree. Site map pages cannot
 * be added through this interface b/c they are brought
 * in using a scan feature that gets all pages and 
 * component pages.
 *
 * @author Janice Gallant for Sonjara, Inc.
 * 
 * date: 10/8/2010
 * 
 ****************************************************************/

Fakoli::using("site_map", "field_renderers", "page");
Fakoli::usingFeature("auto_form");

$menu_item = "Site Map";

$site_map_id = checkNumeric($_GET["site_map_id"]);
$parent_site_map_id = checkNumeric($_GET["parent_site_map_id"]);

if(!$site_map_id)
	redirect("/admin/site_map");
	
$siteMapPage = new SiteMap($site_map_id);

$title = "Edit Details for {$siteMapPage->identifier}";

$form = new AutoForm($siteMapPage);

$redirect = "/admin/site_map";

$form->button("Cancel", $redirect);
$form->submitLabel = "Save Changes";
$form->readOnly("identifier", "page_title", "role");
$identifierSelect = new SelectFieldRenderer($form, "parent_identifier", "Parent Identifier", SiteMap::getSiteMapPageList());


if($_POST['update_order']) 
{  
	$sortOrders = checkNumeric($_POST["subpage"]);
	foreach($sortOrders as $key => $sortOrder)
	{
		$site_map_pg = new SiteMap();
		$site_map_pg->load($key);
		$site_map_pg->save();
	}
	
	// renumber
	$subpages = $siteMapPage->Children();
	$value = 1;
	if(count($subpages) > 0)
	{
		foreach($subpages as $subpage)
		{
			$subpage->filter = new InclusionFilter("sort_order");
			$subpage->sort_order = $value;
			$subpage->save();
			$value++;
		}
	}	
}
elseif ($method == "POST") 
{
	if(validateSiteMapTree($siteMapPage))
	{  
  		$form->save();
		redirect($redirect);
	}
	else
		$form->msg = "<warning>Invalid tree. A page cannot have a parent that is also a child.</warning>";
}


$script = $form->writeScript();

$form->drawForm();

$subpages = $siteMapPage->Children();
if(count($subpages) > 0) 
{	
?>		
	<form name="re-order form" method="post" action="">
<!--  <div style="clear:left;border:solid 1px #000;padding: 4px;width:600px;">  -->
<h3>Sub Pages </h3>
<table class="list" width="600">
 <tr>
  <th>Sort Order</th>
  <th>Title</th>
 </tr>
<? foreach($subpages as $subpage) 
	{
		$parent = $subpage->Parent();  
?>
<tr>                                                                                                                                                   
  <td style="text-align: center; vertical-align: middle; width: 80px">
  <input type="text" style="width: 40px" name="subpage[<? echo $subpage->site_map_id ?>]" value="<? echo $subpage->sort_order ?>"/></td>
  <td>
  <a href="site_page_form.php?page_key=<?echo $subpage->site_map_id ?>&parent_site_map_id=<?echo $parent->site_map_id ?>">
  <?echo $subpage->identifier ?></a></td>	
</tr>	
<? 
	} 
?>
</table>
<!--  </div> -->
<input type="submit" name="update_order" class="button" value="Update Order"/>
<? 
	} 
?>
</form>
<?php 

/*
 * Validate that the tree ancestry and descendents are valid. That is,
 * that no child of this node is also a parent and that no parent
 * is also a child and the parent is not the same identifier as
 * the node.
 */
function validateSiteMapTree($siteMapPage)
{
	global $_POST;
	$valid = true;
	
	$newParent = $_POST["parent_identifier"];

	// don't check if no change in parent
	if($siteMapPage->parent != $newParent)
	{	
		if($siteMapPage->parent_identifier == $newParent)
			$valid = false;
		else
			$valid = validateDescendents($siteMapPage, $newParent, $valid);
	}
	return $valid;
}



/*
 * Recursively create an array of all child pages of $page down to the
 * leaf page. Check if any of the child pages' identifier is the
 * same as the newly selected parent.
 */
function validateDescendents($siteMapPage, $newParent, &$valid)
{
    $children = $siteMapPage->Children();
 
    if($children) 
    {
		foreach($children as $child) 
		{
			trace("validateDescendents:: child identifier {$child->identifier}", 3);
			if($child->identifier == $newParent)
				$valid = false;
			else
				validateDescendents($child, $newParent, $valid);
        }
    }
    return $valid;
}


?>