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

Fakoli::using("admin");

Fakoli::assertRole("admin");

$component = $_GET["src"];
$page = $_GET["page"];

$left = "/action/admin/tree?src={$component}&page={$page}";
$view = "/action/admin/page?src={$component}&page={$page}";
$banner = "/action/admin/banner?src={$component}&page={$page}";

?>
<html>
<frameset rows="35px,*" border="1">
	<frame src="<?echo $banner?>" name="One" scrolling="no" marginwidth="0" marginheight="0" noresize="noresize">
	<frameset cols="30%,70%" border="3">
		<frame src="<?echo $left?>" name="left_nav" scrolling="auto" marginwidth="0" marginheight="0">
		<frame src="<?echo $view?>" name="Two" scrolling="auto" marginwidth="0" marginheight="0">
	</frameset>
</frameset>
</html>
