
/* Merged Plone Javascript file
 * This file is dynamically assembled from separate parts.
 * Some of these parts have 3rd party licenses or copyright information attached
 * Such information is valid for that section,
 * not for the entire composite file
 * originating files are separated by - filename.js -
 */

/* - input-label.js - */
// https://www.ife.no/portal_javascripts/input-label.js?original=1
var ploneInputLabel={focus: function(){var t=jQuery(this);if(t.hasClass('inputLabelActive')&&t.val()==t.attr('title'))
t.val('').removeClass('inputLabelActive');if(t.hasClass('inputLabelPassword'))
ploneInputLabel._setInputType(t.removeClass('inputLabelPassword'),'password').focus().bind('blur.ploneInputLabel',ploneInputLabel.blur)},blur: function(e){var t=jQuery(this);if(t.is(':password[value=""]')){t=ploneInputLabel._setInputType(this,'text').addClass('inputLabelPassword').bind('focus.ploneInputLabel',ploneInputLabel.focus);if(e.originalEvent&&e.originalEvent.explicitOriginalTarget)
jQuery(e.originalEvent.explicitOriginalTarget).trigger('focus!')}
if(!t.val())
t.addClass('inputLabelActive').val(t.attr('title'))},submit: function(){jQuery('input[title].inputLabelActive').trigger('focus.ploneInputLabel')},_setInputType: function(elem,ntype){var $=jQuery;var otype=new RegExp('type="?'+$(elem).attr('type')+'"?')
var nelem=$($('<div></div>').append($(elem).clone()).html().replace(otype,'').replace(/\/?>/,'type="'+ntype+'" />'));$(elem).replaceWith(nelem);return nelem}};(function($){$(function(){$('form:has(input[title].inputLabel)').submit(ploneInputLabel.submit);$('input[title].inputLabel').bind('focus.ploneInputLabel',ploneInputLabel.focus).bind('blur.ploneInputLabel',ploneInputLabel.blur).trigger('blur.ploneInputLabel')})})(jQuery);

/* - jquery.highlightsearchterms.js - */
// https://www.ife.no/portal_javascripts/jquery.highlightsearchterms.js?original=1
(function($){var Highlighter=function(options){$.extend(this,options);this.terms=this.cleanTerms(this.terms.length?this.terms:this.getSearchTerms())};Highlighter.prototype={highlight: function(startnode){if(!this.terms.length||!startnode.length) return;var self=this;$.each(this.terms, function(i,term){startnode.find('*:not(textarea)').andSelf().contents().each(function(){if(this.nodeType==3)
self.highlightTermInNode(this,term)})})},highlightTermInNode: function(node,word){var c=node.nodeValue,self=this;if($(node).parent().hasClass(self.highlightClass)) return;var highlight=function(content){return $('<span class="'+self.highlightClass+'">'+content+'</span>')};var ci=self.caseInsensitive;var index;while(c&&(index=(ci?c.toLowerCase():c).indexOf(word))>-1){$(node).before(document.createTextNode(c.substr(0,index))).before(highlight(c.substr(index,word.length))).before(document.createTextNode(c.substr(index+word.length)));var next=node.previousSibling;$(node).remove();node=next;c=node.nodeValue}},queryStringValue: function(uri,regexp){if(uri.indexOf('?')<0) return '';uri=uri.substr(uri.indexOf('?')+1);while(uri.indexOf('=')>=0){uri=uri.replace(/^\&*/,'');var pair=uri.split('&',1)[0];uri=uri.substr(pair.length);var match=pair.match(regexp);if(match)
return decodeURIComponent(match[match.length-1].replace(/\+/g,' '))}
return ''},termsFromReferrer: function(){var ref=$.fn.highlightSearchTerms._test_referrer!==null?$.fn.highlightSearchTerms._test_referrer:document.referrer;if(!ref) return '';for(var i=0,se;se=this.referrers[i++];){if(ref.match(se.address)) return this.queryStringValue(ref,se.key)}
return ''},cleanTerms: function(terms){var self=this;return $.unique($.map(terms, function(term){term=$.trim(self.caseInsensitive?term.toLowerCase():term);return(!term||self.filterTerms.test(term))?null:term}))},getSearchTerms: function(){var terms=[];var uri=$.fn.highlightSearchTerms._test_location!==null?$.fn.highlightSearchTerms._test_location:location.href;if(this.useReferrer)
$.merge(terms,this.termsFromReferrer().split(/\s+/));if(this.useLocation)
$.merge(terms,this.queryStringValue(uri,this.searchKey).split(/\s+/));return terms}};var makeSearchKey=function(key){return(typeof key==='string')?new RegExp('^'+key+'=(.*)$','i'):key};var makeAddress=function(addr){return(typeof addr==='string')?new RegExp('^https?://(www\\.)?'+addr,'i'):addr};$.fn.highlightSearchTerms=function(options){options=$.extend({},defaults,options);options=$.extend(options,{searchKey:makeSearchKey(options.searchKey),referrers:$.map(options.referrers, function(se){return{address:makeAddress(se.address),key:makeSearchKey(se.key)}})});if(options.includeOwnDomain){var hostname=$.fn.highlightSearchTerms._test_location!==null?$.fn.highlightSearchTerms._test_location:location.hostname;options.referrers.push({address:makeAddress(hostname.replace(/\./g,'\\.')),key:options.searchKey})}
new Highlighter(options).highlight(this);return this};$.fn.highlightSearchTerms.referrers=[{address:'google\\.',key:'q'},{address:'search\\.yahoo\\.',key:'p'},{address:'search\\.msn\\.',key:'q'},{address:'search\\.live\\.',key:'query'},{address:'search\\.aol\\.',key:'userQuery'},{address:'ask\\.com',key:'q'},{address:'altavista\\.',key:'q'},{address:'feedster\\.',key:'q'}];var defaults={terms:[],useLocation:true,searchKey:'(searchterm|SearchableText)',useReferrer:true,referrers:$.fn.highlightSearchTerms.referrers,includeOwnDomain:true,caseInsensitive:true,filterTerms:/(not|and|or)/i,highlightClass:'highlightedSearchTerm'};$.fn.highlightSearchTerms._test_location=null;$.fn.highlightSearchTerms._test_referrer=null;$.fn.highlightSearchTerms._highlighter=Highlighter})(jQuery);
