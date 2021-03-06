<?php

abstract class AutoFormLayout
{	
	var $form;
	
	function AutoFormLayout($form)
	{
		$this->form = $form;
	}
	
	abstract function startGroup($name);
	abstract function endGroup();

	abstract function startField($field, $renderer, $colspan = 1, $styles = "");
	abstract function endField($field, $renderer);
	
	abstract function printLabel($field, $renderer, $styles = "", $annotation = "");
	
	function getLabel($field, $renderer, $addSuffix = true)
	{
		$label = isset($renderer->label) ? $renderer->label : $this->form->prettifyFieldName($field);
		
		if ($renderer->parent->markRequiredFields &&
		    $renderer->parent->isRequired($field))
		{
	    	$label .= "*";
	    }

	    $label .= $renderer->labelSuffix;
		
	    if ($renderer->parent->onFormatLabel)
	    {
	    	$fn = $renderer->parent->onFormatLabel;
	    	$label = $fn($renderer->parent, $field, $label);
	    }
	    
	    return $label;
	}
	
	function getLabelID($field)
	{
		return "{$this->form->id}_{$field}_label";		
	}
	
	abstract function startUngrouped();
	abstract function endUngrouped();
	
	abstract function startButtonLine();
	abstract function endButtonLine();
	
	abstract function errorBox();
	abstract function requiredFields($text);
	
	static function create($type, $form)
	{
		switch($type)
		{
		case "mobile":
			return new MobileFormLayout($form);
							
		case "table_row":
			return new TableRowFormLayout($form);
			
		case "spreadsheet":
			return new SpreadsheetFormLayout($form);
			
		case "simple":
			return new SimpleFormLayout($form);
			
		case "table":
	
		default:
			return new TableBasedFormLayout($form);
		}
	}
}

class TableBasedFormLayout extends AutoFormLayout
{
	function TableBasedFormLayout($form)
	{
		$this->AutoFormLayout($form);
	}
	
	function startGroup($name, $collapsible = false, $collapsed = false)
	{
		$class = "";
		if ($collapsible)
		{
			$class = "collapsible";
			$class .= $collapsed ? " collapsed" : " expanded";
			
			$class = " class='$class'";
		}
		
		$id = $this->form->id . "_" . codify($name) . "_group";
		
		echo "<fieldset id='$id'$class><legend>$name</legend>";
		$this->startUngrouped();
	}
	
	function endGroup()
	{
		$this->endUngrouped();
		echo "</fieldset>";
	}
	
	function startField($field, $renderer, $colspan = 1, $styles = "")
	{
		if ($renderer->colspan > 1) $styles = "text-align: left;$styles";
		
		echo "<tr>\n";
		
		$this->printLabel($field, $renderer, $styles, ($renderer->colspan > 1) ? $this->parent->annotations[$field] : "");
		
		if ($renderer->colspan > 1)
		{
			echo "</tr>\n";
			echo "<tr>\n";
		}
		echo "<td colspan='{$renderer->colspan}'";
		if ($renderer->parent->valueCSS) echo " class='{$renderer->parent->valueCSS}'";
		echo ">";
		
		if ($renderer->annotateBefore && $renderer->parent->annotations[$field] && $renderer->colspan == 1)
		{
			echo $renderer->parent->annotations[$field]."<br/>";
		}
	}
	
	function endField($field, $renderer)
	{
		if (!$renderer->annotateBefore && $renderer->colspan == 1 && $renderer->parent->annotations[$field])
		{
			if ($renderer->annotateNextLine)
			{
				echo "<br/>"; 
			}
			else 
			{
				echo "&nbsp;&nbsp;";
			}
			echo $renderer->parent->annotations[$field];
		}
		
		echo "</td>\n</tr>\n";
	}
	
	/**
	 * Internal method to generate the HTML for the field label
	 *
	 * @param string $field the field name
	 * @param int $colspan the number of columns to span
	 * @param string $styles optional special styles that need to be applied
	 * @param string $annotation annotation text for the field
	 */
	function printLabel($field, $renderer, $styles = "", $annotation = "")
	{		
		$colspan = $renderer->colspan;
		if ($renderer->hideLabel && $colspan > 1 && !$annotation) return;
		
		$label = $this->getLabel($field, $renderer);
		
		echo "<td colspan='$colspan'";
		if ($renderer->parent->labelCSS) echo " class='{$renderer->parent->labelCSS}'";
		if ($styles) echo " style='$styles'";
		echo ">";
		
		if (!$renderer->hideLabel) echo "<label id='".$this->getLabelID($field)."' for='$field'>$label</label> ";
		
		echo "$annotation</td>\n";
	}

	function startUngrouped()
	{
		echo "<table";
		if ($this->form->formCSS) echo " class='{$this->form->formCSS}'";
		if ($this->form->style) echo " style='{$this->form->style}'";
		echo ">\n";
	}
	
	function endUngrouped()
	{
		echo "</table>";
	}
	
	function startButtonLine()
	{
		echo "<tr>\n";
		echo " <td colspan='2' class='submit' style='text-align: {$this->form->buttonAlignment}'><br/>";	
	}
	
	function endButtonLine()
	{
		echo "</td></tr>";
	}
	
	function errorBox()
	{
		echo "<tr>\n <td colspan='2' id='{$this->form->id}__error' class='error' style='display: ".($this->form->msg ? 'table-cell' : 'none')."'>{$this->form->msg}</td></tr>\n";
	}	
	
	function requiredFields($text)
	{
		echo "<tr>\n <td colspan='2'><b>{$text}</b></td></tr>\n";
	}
}

/**
 * Used when you want layout to provide field ordering but
 * don't want groups or borders around the group(s).
 */
class UngroupedFormLayout extends TableBasedFormLayout
{
	function UngroupedFormLayout($form)
	{
		$this->AutoFormLayout($form);
	}
	
	function startGroup($name)
	{
		$this->startUngrouped();
	}
	
	function endGroup()
	{
		$this->endUngrouped();
	}	
}

/*
 * Used for each subordinate form row in 
 * SpreadSheetAutoForm.
 */
class TableRowFormLayout extends AutoFormLayout
{
	function TableRowFormLayout($form)
	{
		$this->AutoFormLayout($form);
	}
	
	function startGroup($name)
	{
		$this->startUngrouped();
	}
	
	function endGroup()
	{
		$this->endUngrouped();
	}
	
			
	function startField($field, $renderer, $colspan = 1, $styles = "")
	{
		echo "<td ";
		if ($renderer->parent->valueCSS) echo " class='{$renderer->parent->valueCSS}'";
		echo ">";
		
		if ($renderer->annotateBefore && $renderer->parent->annotations[$field] && $renderer->colspan == 1)
		{
			echo $renderer->parent->annotations[$field]."<br/>";
		}		
	}
	
	function endField($field, $renderer)
	{
		echo "</td>\n";
	}
	
	function requiredFields($text)
	{
		// empty - want displayed once at top not for every form in the spreadsheet
	}	
	
	/**
	 * Internal method to generate the HTML for the field label
	 *
	 * @param string $field the field name
	 * @param int $colspan the number of columns to span
	 * @param string $styles optional special styles that need to be applied
	 * @param string $annotation annotation text for the field
	 */
	function printLabel($field, $renderer, $styles = "", $annotation = "")
	{		
		if ($renderer->hideLabel && $renderer->colspan > 1 && !$annotation) return;
		
		$label = $this->getLabel($field, $renderer);
		
		echo "<th ";
		if ($renderer->parent->labelCSS) echo " class='{$renderer->parent->labelCSS}'";
		if ($styles) echo " style='$styles'";
		echo ">";
		
		if (!$renderer->hideLabel) echo "<label id='".$this->getLabelID($field)."' for='$field'>$label</label> ";
		
		echo "$annotation</th>\n";
	}
	
	function getLabel($field, $renderer, $styles, $annotation)
	{
		$obj = $renderer->parent->getData();
		$label = isset($renderer->label) ? $renderer->label : $obj->prettifyFieldName($field);
		
		if ($renderer->parent->markRequiredFields &&
		    $renderer->parent->isRequired($field))
		{
	    	$label .= "*";
	    }

	    $label .= $renderer->labelSuffix;
		
	    if ($renderer->parent->onFormatLabel)
	    {
	    	$fn = $renderer->parent->onFormatLabel;
	    	$label = $fn($renderer->parent, $field, $label);
	    }
	    
	    return $label;
	}
	
	function startUngrouped()
	{
		echo "<tr id='{$this->form->id}_container' class='{$this->form->containerClass}'>\n";
	}
	
	function endUngrouped()
	{		
		echo "</tr>\n";
	}
	
	function startButtonLine()
	{
		// nothing to do
	}
	
	function endButtonLine()
	{
		// nothing to do
	}
	
	function errorBox()
	{
		echo "<tr>\n <td colspan='{$this->form->parent->colCount}' id='{$this->form->id}__error' class='error' style='display: ".($this->form->msg ? 'table-cell' : 'none')."'>{$this->form->msg}</td></tr>\n";
	}	
}

/*
 * Used for the overall layout handling of SpreadsheetAutoForm
 * instance.
 */
class SpreadsheetFormLayout extends AutoFormLayout
{
	function SpreadsheetFormLayout($form)
	{
		$this->AutoFormLayout($form);
	}
	
	function startGroup($name)
	{
		$this->startUngrouped();
	}
	
	function endGroup()
	{
		$this->endUngrouped();
	}
	
	function startField($field, $renderer, $colspan = 1, $styles = "")
	{
		// nothing do to
	}
	
	function endField($field, $renderer)
	{
		// nothing do to
	}
	
	function requiredFields($text)
	{
		echo "<p><b>$text</b></p>";	
	}	
	
	function printLabel($field, $renderer, $styles = "", $annotation = "")
	{
		// nothing to do		
	}
	
	function getLabel($field, $renderer, $styles, $annotation)
	{
		// nothing to do
	}
	
	function startUngrouped()
	{
		echo "<table ";
		if ($this->form->formCSS) echo " class='{$this->form->formCSS}'";
		if ($this->form->style) echo " style='{$this->form->style}'";
		echo ">\n";
	}
	
	function endUngrouped()
	{
		echo "</table>";
	}
	
	function startButtonLine()
	{
		echo "<tr>\n";
		echo " <td colspan='{$this->form->colCount}' class='submit' style='text-align: {$this->form->buttonAlignment}'><br/>";	
	}
	
	function endButtonLine()
	{
		echo "</td></tr>";
	}
	
	function errorBox()
	{
		// nothing to do
	}	
}

/**
 * MobileFormLayout provides a linear form layout suitable for mobile devices.
 */
class MobileFormLayout extends AutoFormLayout
{
	function MobileFormLayout($form)
	{
		$this->AutoFormLayout($form);
	}
	
	function startGroup($name, $collapsible = false, $collapsed = false)
	{
		$class = "";
		if ($collapsible)
		{
			$class = "collapsible";
			$class .= $collapsed ? " collapsed" : " expanded";
			
			$class = " class='$class'";
		}
		
		$id = $this->form->id . "_" . codify($name) . "_group";
		
		echo "<fieldset id='$id'$class><legend>$name</legend>";
		$this->startUngrouped();
	}
	
	function endGroup()
	{
		$this->endUngrouped();
		echo "</fieldset>";
	}
	
	function startField($field, $renderer, $colspan = 1, $styles = "")
	{	
		echo "<br/>\n";
		
		$this->printLabel($field, $renderer, $styles, $this->parent->annotations[$field]);
		
		echo "<br/>\n";
		
		if ($renderer->annotateBefore && $renderer->parent->annotations[$field])
		{
			echo $renderer->parent->annotations[$field]."<br/>";
		}
	}
	
	function endField($field, $renderer)
	{
		if (!$renderer->annotateBefore && $renderer->parent->annotations[$field])
		{
			if ($renderer->annotateNextLine)
			{
				echo "<br/>"; 
			}
			else 
			{
				echo "&nbsp;&nbsp;";
			}
			echo $renderer->parent->annotations[$field];
		}
		
		echo "<br/>\n";
	}
	
	/**
	 * Internal method to generate the HTML for the field label
	 *
	 * @param string $field the field name
	 * @param int $colspan the number of columns to span
	 * @param string $styles optional special styles that need to be applied
	 * @param string $annotation annotation text for the field
	 */
	function printLabel($field, $renderer, $styles = "", $annotation = "")
	{		
		$colspan = $renderer->colspan;
		if ($renderer->hideLabel && !$annotation) return;
		
		$label = $this->getLabel($field, $renderer);
		
		if (!$renderer->hideLabel)
		{
			echo "<label id='".$this->getLabelID($field)."' for='$field'";
			if ($renderer->parent->labelCSS) echo " class='{$renderer->parent->labelCSS}'";
			if ($styles) echo " style='$styles'";
		
			echo ">$label</label> ";
		}
		
		echo "$annotation\n";
	}

	function startUngrouped()
	{
		echo "<div";
		if ($this->form->formCSS) echo " class='{$this->form->formCSS}'";
		if ($this->form->style) echo " style='{$this->form->style}'";
		echo ">\n";
	}
	
	function endUngrouped()
	{
		echo "</div>";
	}
	
	function startButtonLine()
	{
		echo "<br/>\n";
		echo " <div class='submit' style='text-align: {$this->form->buttonAlignment}'><br/>";	
	}
	
	function endButtonLine()
	{
		echo "</div><br/>";
	}
	
	function errorBox()
	{
		echo "<div id='{$this->form->id}__error' class='error' style='display: ".($this->form->msg ? 'table-cell' : 'none')."'>{$this->form->msg}</div>\n";
	}	
	
	function requiredFields($text)
	{
		echo "<p style='font-weight: bold'>{$text}</p>\n";
	}
}




/**
 * SimpleFormLayout provides a form with minimal HTML, and does not employ table-based layout.
 */
class SimpleFormLayout extends AutoFormLayout
{
	function MobileFormLayout($form)
	{
		$this->AutoFormLayout($form);
	}
	
	function startGroup($name, $collapsible = false, $collapsed = false)
	{
		$class = "";
		if ($collapsible)
		{
			$class = "collapsible";
			$class .= $collapsed ? " collapsed" : " expanded";
			
			$class = " class='$class'";
		}
		
		$id = $this->form->id . "_" . codify($name) . "_group";
		
		echo "<fieldset id='$id'$class><legend>$name</legend>";
		$this->startUngrouped();
	}
	
	function endGroup()
	{
		$this->endUngrouped();
		echo "</fieldset>";
	}
	
	function startField($field, $renderer, $colspan = 1, $styles = "")
	{	
		echo "<br/>\n";
		
		$this->printLabel($field, $renderer, $styles, $this->parent->annotations[$field]);
		
		if ($renderer->annotateBefore && $renderer->parent->annotations[$field])
		{
			echo $renderer->parent->annotations[$field]." ";
		}

		if ($colspan == 2) echo "<br/>\n";
	}
	
	function endField($field, $renderer)
	{
		if (!$renderer->annotateBefore && $renderer->parent->annotations[$field])
		{
			if ($renderer->annotateNextLine)
			{
				echo "<br/>"; 
			}
			else 
			{
				echo "&nbsp;&nbsp;";
			}
			echo $renderer->parent->annotations[$field];
		}
		
		echo "<br/>\n";
	}
	
	/**
	 * Internal method to generate the HTML for the field label
	 *
	 * @param string $field the field name
	 * @param int $colspan the number of columns to span
	 * @param string $styles optional special styles that need to be applied
	 * @param string $annotation annotation text for the field
	 */
	function printLabel($field, $renderer, $styles = "", $annotation = "")
	{		
		$colspan = $renderer->colspan;
		if ($renderer->hideLabel && !$annotation) return;
		
		$label = $this->getLabel($field, $renderer);
		
		if (!$renderer->hideLabel)
		{
			echo "<label id='".$this->getLabelID($field)."' for='$field'";
			if ($renderer->parent->labelCSS) echo " class='{$renderer->parent->labelCSS}'";
			if ($styles) echo " style='$styles'";
		
			echo ">$label</label> ";
		}
		
		echo "$annotation\n";
	}

	function startUngrouped()
	{
		echo "<div";
		if ($this->form->formCSS) echo " class='{$this->form->formCSS}'";
		if ($this->form->style) echo " style='{$this->form->style}'";
		echo ">\n";
	}
	
	function endUngrouped()
	{
		echo "</div>";
	}
	
	function startButtonLine()
	{
		echo "<br/>\n";
		echo " <div class='submit' style='text-align: {$this->form->buttonAlignment}'><br/>";	
	}
	
	function endButtonLine()
	{
		echo "</div><br/>";
	}
	
	function errorBox()
	{
		echo "<div id='{$this->form->id}__error' class='error' style='display: ".($this->form->msg ? 'table-cell' : 'none')."'>{$this->form->msg}</div>\n";
	}	
	
	function requiredFields($text)
	{
		echo "<p style='font-weight: bold'>{$text}</p>\n";
	}
}

/**
 * Layout for list filtering using filter_form.inc.
 * This layout is not table-based; it used float left
 * divs to layout the filter fields left to right.
 * 
 * @author janice
 */
class FilterFormLayout extends AutoFormLayout
{
	function FilterFormLayout($form)
	{
		$this->AutoFormLayout($form);
	}
	
	function startGroup($name, $collapsible = false, $collapsed = false)
	{
		$class = "";
		if ($collapsible)
		{
			$class = "collapsible";
			$class .= $collapsed ? " collapsed" : " expanded";
			
			$class = " class='$class'";
		}
		
		$id = $this->form->id . "_" . codify($name) . "_group";
		
		$this->startUngrouped();
	}
	
	function endGroup()
	{
		$this->endUngrouped();
	}
	
	function startField($field, $renderer, $colspan = 1, $styles = "")
	{
		echo "<div class='{$renderer->cssClass}'>\n";
		$this->printLabel($field, $renderer, $styles, "");
	}
	
	function endField($field, $renderer)
	{
		echo "</div>\n";
	}
	
	/**
	 * Internal method to generate the HTML for the field label
	 *
	 * @param string $field the field name
	 * @param int $colspan the number of columns to span
	 * @param string $styles optional special styles that need to be applied
	 * @param string $annotation annotation text for the field
	 */
	function printLabel($field, $renderer, $styles = "", $annotation = "")
	{		
		if ($renderer->hideLabel) return;
		
		$label = $this->getLabel($field, $renderer);
		
		if (!$renderer->hideLabel) echo "<label id='".$this->getLabelID($field)."' for='$field'>$label</label> ";
	}

	function startUngrouped()
	{
		// nothing to do
	}
	
	function endUngrouped()
	{
		// nothing to do
	}
	
	function startButtonLine()
	{
		// nothing to do
	}
	
	function endButtonLine()
	{
		// nothing to do
	}
	
	function errorBox()
	{
		// nothing to do
	}	
	
	function requiredFields($text)
	{
		// nothing to do
	}
}