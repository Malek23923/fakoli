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

Fakoli::usingFile("framework/tab_bar.inc");

function menuTabs($key)
{
	$tabs = array(	"Menu Details"	=>	"menu_form",
				    "Menu Items"	=>	"menu_items"
				 );
					
   
	$qs = ($key) ? "menu_id=$key" : "";
	return new TabBar("tabs", $tabs, $qs);
}

function menuAdminBreadcrumbs($menuItem, $menu)
{
	$breadcrumb = array();
	if ($menuItem)
	{
		$breadcrumb[] = ($menuItem->title) ? $menuItem->title : "New Node";
		
		
		while($menuItem->parent_id)
		{
			$menuItem = $menuItem->Parent();
			
			$breadcrumb[] = "<a href='menu_item_form?menu_item_id={$menuItem->menu_item_id}'>".(($menuItem->title) ? $menuItem->title : "New Node")."</a>";
		}
	}
		
	$breadcrumb[] = "<a href='menu_form?menu_id={$menu->menu_id}'>{$menu->name}</a>";
	$breadcrumb = array_reverse($breadcrumb);
	
	return implode("&nbsp;&raquo;&nbsp;", $breadcrumb);
}
?>