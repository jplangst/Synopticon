
/* Merged Plone Javascript file
 * This file is dynamically assembled from separate parts.
 * Some of these parts have 3rd party licenses or copyright information attached
 * Such information is valid for that section,
 * not for the entire composite file
 * originating files are separated by - filename.js -
 */

/* - nodeutilities.js - */
// https://www.ife.no/portal_javascripts/nodeutilities.js?original=1
function wrapNode(node,wrappertype,wrapperclass){jQuery(node).wrap('<'+wrappertype+'>').parent().addClass(wrapperclass)};
function nodeContained(innernode,outernode){return jQuery(innernode).parents().filter(function(){return this==outernode}).length>0};
function findContainer(node,func){p=jQuery(node).parents().filter(func);return p.length?p.get(0):false};
function hasClassName(node,class_name){return jQuery(node).hasClass(class_name)};
function addClassName(node,class_name){jQuery(node).addClass(class_name)};
function removeClassName(node,class_name){jQuery(node).removeClass(class_name)};
function replaceClassName(node,old_class,new_class,ignore_missing){if(ignore_missing||jQuery(node).hasClass(old_class))
jQuery(node).removeClass(old_class).addClass(new_class)};
function walkTextNodes(node,func,data){jQuery(node).find('*').andSelf().contents().each(function(){if(this.nodeType==3) func(this,data)})};
function getInnerTextCompatible(node){return jQuery(node).text()};
function getInnerTextFast(node){return jQuery(node).text()};
function sortNodes(nodes,fetch_func,cmp_func){var SortNodeWrapper=function(node){this.value=fetch_func(node);this.cloned_node=node.cloneNode(true)}
SortNodeWrapper.prototype.toString=function(){return this.value.toString?this.value.toString():this.value}
var items=jQuery(nodes).map(function(){return new SortNodeWrapper(this)});if(cmp_func) items.sort(cmp_func);else items.sort();jQuery.each(items, function(i){jQuery(nodes[i]).replace(this.cloned_node)})};
function copyChildNodes(srcNode,dstNode){jQuery(srcNode).children().clone().appendTo(jQuery(dstNode))}


/* - cookie_functions.js - */
// https://www.ife.no/portal_javascripts/cookie_functions.js?original=1
function createCookie(name,value,days){if(days){var date=new Date();date.setTime(date.getTime()+(days*24*60*60*1000));var expires="; expires="+date.toGMTString()} else{expires=""}
document.cookie=name+"="+escape(value)+expires+"; path=/;"};
function readCookie(name){var nameEQ=name+"=";var ca=document.cookie.split(';');for(var i=0;i<ca.length;i++){var c=ca[i];while(c.charAt(0)==' '){c=c.substring(1,c.length)}
if(c.indexOf(nameEQ)==0){return unescape(c.substring(nameEQ.length,c.length))}}
return null};

/* - livesearch.js - */
// https://www.ife.no/portal_javascripts/livesearch.js?original=1
var livesearch=function(){var _2=400;var _7=400;var _0={};var _1="LSHighlight";function _5(f,i){var l=null;var r=null;var c={};var q="livesearch_reply";var q=f.attr('action').replace(/search$/g,"")+q;var re=f.find('div.LSResult');var s=f.find('div.LSShadow');var p=f.find('input[name=path]');function _12(){jQuery(re).find('.livesearchContainer, .LSIEFix').slideUp('fast',function(){jQuery(re).find('.toolTipArrow').fadeOut(100,function(){re.hide();jQuery(re).find('.toolTipArrow').show()})});l=null};function _6(){window.setTimeout('livesearch.hide("'+f.attr('id')+'")',_7)};function _11(d){if((jQuery(re).find('.livesearchContainer').length===0)||jQuery(re).find('.livesearchContainer').is(':hidden')){s.html(d);jQuery(re).find('.livesearchContainer').hide();re.show();jQuery('.livesearchContainer').animate({opacity:'toggle',height:'toggle'},'fast','swing')}else{re.show();s.html(d)}};function _14(){if(l==i.value){return}l=i.value;if(r&&r.readyState<4)r.abort();if(i.value.length<2){_12();return}var qu={q:i.value};if(p.length&&p[0].checked)qu['path']=p.val();qu=jQuery.param(qu);if(c[qu]){_11(c[qu]);return}r=jQuery.get(q,qu,function(d){_11(d);c[qu]=d},'text')};function _4(){window.setTimeout('livesearch.search("'+f.attr('id')+'")',_2)};return{hide:_12,hide_delayed:_6,search:_14,search_delayed:_4}};function _3(f){var t=null;var re=f.find('div.LSResult');var s=f.find('div.LSShadow');function _16(){c=s.find('li.LSHighlight').removeClass(_1);p=c.prev('li');if(!p.length)p=s.find('li:last');p.addClass(_1);return false};function _9(){c=s.find('li.LSHighlight').removeClass(_1);n=c.next('li');if(!n.length)n=s.find('li:first');n.addClass(_1);return false};function _8(){s.find('li.LSHighlight').removeClass(_1);re.hide()};function _10(e){window.clearTimeout(t);switch(e.keyCode){case 38:return _16();case 40:return _9();case 27:return _8();case 37:break;case 39:break;default:{t=window.setTimeout('livesearch.search("'+f.attr('id')+'")',_2)}}};function _13(){var t=s.find('li.LSHighlight a').attr('href');if(!t)return;window.location=t;return false};return{handler:_10,submit:_13}};function _15(i){var i='livesearch'+i;var f=jQuery(this).parents('form:first');var k=_3(f);_0[i]=_5(f,this);f.attr('id',i).css('white-space','nowrap').submit(k.submit);jQuery(this).attr('autocomplete','off').keydown(k.handler).focus(_0[i].search_delayed).blur(_0[i].hide_delayed)};jQuery(function(){jQuery("#searchGadget,input.portlet-search-gadget").each(_15)});return{search:function(id){_0[id].search()},hide:function(id){_0[id].hide()}}}();

/* - select_all.js - */
// https://www.ife.no/portal_javascripts/select_all.js?original=1
function toggleSelect(selectbutton,id,initialState,formName){id=id||'ids:list'
var state=selectbutton.isSelected;state=state==null?Boolean(initialState):state;selectbutton.isSelected=!state;jQuery(selectbutton).attr('src',portal_url+'/select_'+(state?'all':'none')+'_icon.png');var base=formName?jQuery(document.forms[formName]):jQuery(document);base.find(':checkbox[name='+id+']').attr('checked',!state)}


/* - dragdropreorder.js - */
// https://www.ife.no/portal_javascripts/dragdropreorder.js?original=1
var ploneDnDReorder={};ploneDnDReorder.dragging=null;ploneDnDReorder.table=null;ploneDnDReorder.rows=null;(function($){ploneDnDReorder.doDown=function(e){var dragging=$(this).parents('.draggable:first');if(!dragging.length) return;ploneDnDReorder.rows.mousemove(ploneDnDReorder.doDrag);ploneDnDReorder.dragging=dragging;dragging._position=ploneDnDReorder.getPos(dragging);dragging.addClass("dragging");$(this).parents('tr').addClass('dragindicator');return false};ploneDnDReorder.getPos=function(node){var pos=node.parent().children('.draggable').index(node[0]);return pos==-1?null:pos};ploneDnDReorder.doDrag=function(e){var dragging=ploneDnDReorder.dragging;if(!dragging) return;var target=this;if(!target) return;if($(target).attr('id')!=dragging.attr('id')){ploneDnDReorder.swapElements($(target),dragging)};return false};ploneDnDReorder.swapElements=function(child1,child2){var parent=child1.parent();var items=parent.children('[id]');items.removeClass('even').removeClass('odd');if(child1[0].swapNode){child1[0].swapNode(child2[0])} else{var t=parent[0].insertBefore(document.createTextNode(''),child1[0]);child1.insertBefore(child2);child2.insertBefore(t);$(t).remove()};parent.children('[id]:odd').addClass('even');parent.children('[id]:even').addClass('odd')};ploneDnDReorder.doUp=function(e){var dragging=ploneDnDReorder.dragging;if(!dragging) return;dragging.removeClass("dragging");ploneDnDReorder.updatePositionOnServer();dragging._position=null;try{delete dragging._position} catch(e){};dragging=null;ploneDnDReorder.rows.unbind('mousemove',ploneDnDReorder.doDrag);$(this).parents('tr').removeClass('dragindicator');return false};ploneDnDReorder.updatePositionOnServer=function(){var dragging=ploneDnDReorder.dragging;if(!dragging) return;var delta=ploneDnDReorder.getPos(dragging)-dragging._position;if(delta==0){return};var args={item_id:dragging.attr('id').substr('folder-contents-item-'.length)};args['delta:int']=delta;jQuery.post('folder_moveitem',args)}})(jQuery);

/* - mark_special_links.js - */
// https://www.ife.no/portal_javascripts/mark_special_links.js?original=1
function scanforlinks(){if(typeof external_links_open_new_window=='string')
var elonw=external_links_open_new_window.toLowerCase()=='true';else elonw=false;if(typeof mark_special_links=='string')
var mslinks=mark_special_links.toLowerCase()=='true';else mslinks=false;var url=window.location.protocol+'//'+window.location.host;if(elonw)
jQuery('a[href^=http]:not(.link-plain):not([href^='+url+'])').attr('target','_blank');if(mslinks){var protocols=/^(mailto|ftp|news|irc|h323|sip|callto|https|feed|webcal)/;var contentarea=jQuery(getContentArea());contentarea.find('a[href^=http]:not(.link-plain):not([href^='+url+']):not(:has(img))').wrap('<span></span>').parent().addClass('link-external')
contentarea.find('a[href]:not([href^=http]):not(.link-plain):not([href^='+url+']):not(:has(img))').each(function(){if(res=protocols.exec(this.href))
jQuery(this).wrap('<span></span>').parent().addClass('link-'+res[0])})}};jQuery(scanforlinks);

/* - collapsiblesections.js - */
// https://www.ife.no/portal_javascripts/collapsiblesections.js?original=1
function activateCollapsibles(){(function($){$('dl.collapsible:not([class$=Collapsible])').find('dt.collapsibleHeader:first').click(function(){var c=$(this).parents('dl.collapsible:first');if(!c)return true;var t=c.hasClass('inline')?'Inline':'Block';c.toggleClass('collapsed'+t+'Collapsible').toggleClass('expanded'+t+'Collapsible')}).end().each(function(){var s=$(this).hasClass('collapsedOnLoad')?'collapsed':'expanded';var t=$(this).hasClass('inline')?'Inline':'Block';$(this).removeClass('collapsedOnLoad').addClass(s+t+'Collapsible')})})(jQuery)};(function($){$(activateCollapsibles)})(jQuery);

/* - collapsibleformfields.js - */
// https://www.ife.no/portal_javascripts/collapsibleformfields.js?original=1
(function($){$.fn.do_search_collapse=function(){return this.each(
function(){
function check_used(element){e=$(element);if(e.find('input[id$=_toggle]:checkbox').length>0){if(e.find('input[id$=_toggle]:checkbox:checked').length==0){return true}};if(e.find(':text[value]').length>0){return true};if(e.find('select .default_option').length>0){if(e.find('select .default_option:selected').length==0){return true}}
return false};var indicator=$(this).find('.collapser:first');var collapse=$(this).find('.collapse:first');indicator.click(function(){var container=$(this).parent();target=container.find('.collapse:first');target.slideToggle('normal');$(this).toggleClass('expanded');$(this).toggleClass('collapsed')});if(check_used(this)){indicator.addClass('expanded')} else{collapse.hide();indicator.addClass('collapsed')}})}})(jQuery);

/* - form_tabbing.js - */
// https://www.ife.no/portal_javascripts/form_tabbing.js?original=1
var ploneFormTabbing={jqtConfig:{current:'selected'}};(function($){ploneFormTabbing._buildTabs=function(container,legends){var threshold=legends.length>6;var panel_ids,tab_ids=[],tabs='';for(var i=0;i<legends.length;i++){var className,tab,legend=legends[i],lid=legend.id;tab_ids[i]='#'+lid;switch(i){case(0):className='class="formTab firstFormTab"';break;case(legends.length-1):className='class="formTab lastFormTab"';break;default:className='class="formTab"';break}
if(threshold){tab='<option '+className+' id="'+lid+'" value="'+lid+'">';tab+=$(legend).text()+'</option>'} else{tab='<li '+className+'><a id="'+lid+'" href="#'+lid+'"><span>';tab+=$(legend).text()+'</span></a></li>'}
tabs+=tab;$(legend).hide()}
tab_ids=tab_ids.join(',');panel_ids=tab_ids.replace(/#fieldsetlegend-/g,"#fieldset-");if(threshold){tabs=$('<select class="formTabs">'+tabs+'</select>');tabs.change(function(){var selected=$(this).attr('value');jq('#'+selected).click()})} else{tabs=$('<ul class="formTabs">'+tabs+'</ul>')}
return tabs};ploneFormTabbing.initializeDL=function(){var ftabs=$(ploneFormTabbing._buildTabs(this,$(this).children('dt')));var targets=$(this).children('dd');$(this).before(ftabs);targets.addClass('formPanel');ftabs.tabs(targets,ploneFormTabbing.jqtConfig)};ploneFormTabbing.initializeForm=function(){var jqForm=$(this);var fieldsets=jqForm.children('fieldset');if(!fieldsets.length){return}
var ftabs=ploneFormTabbing._buildTabs(this,fieldsets.children('legend'));$(this).prepend(ftabs);fieldsets.addClass("formPanel");$(this).find('input[name=fieldset.current]').addClass('noUnloadProtection');$(this).find('.formPanel:has(div.field span.required)').each(function(){var id=this.id.replace(/^fieldset-/,"#fieldsetlegend-");$(id).addClass('required')});var initialIndex=0;var count=0;var found=false;$(this).find('.formPanel').each(function(){if(!found&&$(this).find('div.field.error').length!=0){initialIndex=count;found=true}
count+=1});var tabSelector='ul.formTabs';if($(ftabs).is('select.formTabs')){tabSelector='select.formTabs'}
var tabsConfig=$.extend({},ploneFormTabbing.jqtConfig,{'initialIndex':initialIndex});jqForm.children(tabSelector).tabs('form.enableFormTabbing fieldset.formPanel',tabsConfig);jqForm.submit(function(){var selected;if(ftabs.find('a.selected').length>=1){selected=ftabs.find('a.selected').attr('href').replace(/^#fieldsetlegend-/,"#fieldset-")}
else{selected=ftabs.attr('value').replace(/^fieldsetlegend-/,'#fieldset-')}
var fsInput=jqForm.find('input[name=fieldset.current]');if(selected&&fsInput){fsInput.val(selected)}});$("#archetypes-schemata-links").addClass('hiddenStructure');$("div.formControls input[name=form.button.previous],"+"div.formControls input[name=form.button.next]").remove()};$.fn.ploneTabInit=function(pbo){return this.each(function(){var item=$(this);item.find("form.enableFormTabbing,div.enableFormTabbing").each(ploneFormTabbing.initializeForm);item.find("dl.enableFormTabbing").each(ploneFormTabbing.initializeDL);var targetPane=item.find('.enableFormTabbing input[name=fieldset.current]').val()||window.location.hash;if(targetPane){item.find(".enableFormTabbing .formtab a[href='"+targetPane.replace("'","").replace(/^#fieldset-/,"#fieldsetlegend-")+"']").click()}})};ploneFormTabbing.initialize=function(){$('body').ploneTabInit()}})(jQuery);jQuery(function(){ploneFormTabbing.initialize()});

/* XXX ERROR -- could not find 'se-highlight.js'*/

/* - first_input_focus.js - */
// https://www.ife.no/portal_javascripts/first_input_focus.js?original=1
(function($){$(function(){if($("form div.error :input:first").focus().length) return;$("form.enableAutoFocus :input:not(.formTabs):visible:first").focus()})})(jQuery);

/* - accessibility.js - */
// https://www.ife.no/portal_javascripts/accessibility.js?original=1
function setBaseFontSize(f,r){var b=jQuery('body');if(r){b.removeClass('smallText').removeClass('largeText');createCookie("fontsize",f,365)}b.addClass(f)};(function($){$(function(){var f=readCookie("fontsize");if(f)setBaseFontSize(f,0)})})(jQuery);

/* - styleswitcher.js - */
// https://www.ife.no/portal_javascripts/styleswitcher.js?original=1
function setActiveStyleSheet(title,reset){jQuery('link[rel*=style][title]').attr('disabled',true).find('[title='+title+']').attr('disabled',false);if(reset) createCookie("wstyle",title,365)};jQuery(function(){var style=readCookie("wstyle");if(style!=null) setActiveStyleSheet(style,0)});

/* - toc.js - */
// https://www.ife.no/portal_javascripts/toc.js?original=1
(function($){$(function(){var dest=$('dl.toc dd.portletItem');var content=getContentArea();if(!content||!dest.length) return;dest.empty();var location=window.location.href;if(window.location.hash)
location=location.substring(0,location.lastIndexOf(window.location.hash));var stack=[];$(content).find('*').not('.comment > h3').filter(function(){return/^h[1234]$/.test(this.tagName.toLowerCase())}).not('.documentFirstHeading').each(function(i){var level=this.nodeName.charAt(1);while(stack.length<level){var ol=$('<ol>');if(stack.length){var li=$(stack[stack.length-1]).children('li:last');if(!li.length)
li=$('<li>').appendTo($(stack[stack.length-1]));li.append(ol)}
stack.push(ol)}
while(stack.length>level) stack.pop();$(this).before($('<a name="section-'+i+'" />'));$('<li>').append($('<a />').attr('href',location+'#section-'+i).text($(this).text())).appendTo($(stack[stack.length-1]))});if(stack.length){$('dl.toc').show();var oltoc=$(stack[0]);var i=1;while(oltoc.children('li').length==1){oltoc=$(stack[i]);i+=1}
numdigits=oltoc.children().length.toString().length;oltoc.addClass("TOC"+numdigits+"Digit");dest.append(oltoc);var wlh=window.location.hash;if(wlh){var target=$(wlh);target=target.length&&target||$('[name='+wlh.slice(1)+']');var targetOffset=target.offset();if(targetOffset)
$('html,body').animate({scrollTop:targetOffset.top},0)}}})})(jQuery);
