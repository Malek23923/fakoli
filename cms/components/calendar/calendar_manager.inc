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

Fakoli::using("component");
Fakoli::usingFeature("tab_bar");

class CalendarManager
{
	static function displayCalendar($identifier, &$continue)
	{
		try
		{
			$calendar = Calendar::findByIdentifier($identifier);
			
			$page = ComponentPage::findByIdentifier("calendar_view", "WHERE enabled=1");
			$_GET["cal"] = $identifier;
		
			$pageView = new ComponentPageView($page, "{$page->template}.tpl");
		
			$page_role = $page->role;
			
			if (!checkRole($page->role))
			{
				LoginManager::storeRedirectPage();
				redirect("/login");
			}
			
			echo $pageView->drawView();
			
			$continue = false;
		}
		catch(DataNotFoundException $e)
		{
			
		}
		
		return $identifier;
	}
	
	static function enumerateItems($items)
	{
		$calendars = Query::create(Calendar, "ORDER BY name")->execute();
		
		$items["Calendars"] = $calendars;
		return $items;
	}
	
	static function enumerateContextHelpClasses($classes)
	{
		array_push($classes, "Event");
		
		return $classes;
	}
	
	static function getTabs($key)
	{
		$tabs = array(	"Details"			=>	"/admin/calendar_form",
				"Calendar"			=>	"/admin/calendar_view",
				"Event List"		=>	"/admin/calendar_events"
		);
	
		$qs = ($key) ? "calendar_id=$key" : "";
		return new TabBar("tabs", $tabs, $qs);
	}
	
	static function upgradeComponent($version)
	{
		$mgr = new CalendarUpgradeManager();
		$mgr->upgrade($version);
	}	
	
	static function calendarGetHandler()
	{
		$calendarHandler = new CalendarHandler();
		return $calendarHandler;
	}

	static function registerTaxonomyClasses($classes)
	{
		$classes[] = Event;
		return $classes;
	}
}

?>