
/* Merged Plone Javascript file
 * This file is dynamically assembled from separate parts.
 * Some of these parts have 3rd party licenses or copyright information attached
 * Such information is valid for that section,
 * not for the entire composite file
 * originating files are separated by - filename.js -
 */

/* - explorer.js - */
/*
jQuery portletNavigationTree plugin
	Collapsible/expandable navigation tree.
*/

(function($) {
	// Expands or collapses a node of which sub-items already have been loaded.
	function toggleNode(event){
		var twistie = $(this);
		
		// find ul element
		var ul = $(this).closest("li").children("ul");
		if (!ul) { return; }
		
		// toggle class names
		if (twistie.hasClass("showChildren")){
			ul.removeClass("hideChildren");
			twistie.removeClass("showChildren");
		} else {
			ul.addClass("hideChildren");
			twistie.addClass("showChildren");
		}
		
		// prevent default action of event
		event.preventDefault();
	}
	
	// Loads the sub-items of a node.
	function loadNode(event){
	
		// prevent default action of event
		event.preventDefault();
		
		// Find the portlet information
		var portletWrapper = $(this).closest(".portletWrapper");
		var portletHash = portletWrapper[0] ? portletWrapper[0].id.replace("portletwrapper-","") : "";
		if (!portletHash) { return; }

		// find the li element of the clicked node
		var node = $(this).closest("li");
		
		if (node.hasClass("nodeLoading")) { return; }
		
		// get node uid
		var uidClassName = node[0].className.match(/node-(\w+)/);
		var uid = uidClassName ? uidClassName[1] : null;
		if (!uid) { return; }
		
		// data to send with request
		var data = {
			portlethash : portletHash,
			uid : uid
		};
		
		// add nodeLoading class
		node.addClass("nodeLoading");
		
		// send request
		$.post("expandNode", data, function(html){
			node.replaceWith(html);
		});
	}

	// observe clicks on toggle buttons
	$(".portletNavigationTree span.toggleNode").live("click", loadNode);
	$(".portletNavigationTree span.expandedNode").live("click", toggleNode);
})(jQuery);


/* - modernizr.js - */
// https://www.ife.no/portal_javascripts/modernizr.js?original=1
window.Modernizr=function(i,e,u){function s(a,b){return(""+a).indexOf(b)!==-1}function D(a,b){for(var c in a)if(j[a[c]]!==u&&(!b||b(a[c],E)))return true}function n(a,b){var c=a.charAt(0).toUpperCase()+a.substr(1);c=(a+" "+F.join(c+" ")+c).split(" ");return!!D(c,b)}function S(){f.input=function(a){for(var b=0,c=a.length;b<c;b++)L[a[b]]=!!(a[b]in h);return L}("autocomplete autofocus list placeholder max min multiple pattern required step".split(" "));f.inputtypes=function(a){for(var b=0,c,k=a.length;b<k;b++){h.setAttribute("type",a[b]);if(c=h.type!=="text"){h.value=M;if(/^range$/.test(h.type)&&h.style.WebkitAppearance!==u){l.appendChild(h);c=e.defaultView;c=c.getComputedStyle&&c.getComputedStyle(h,null).WebkitAppearance!=="textfield"&&h.offsetHeight!==0;l.removeChild(h)}else/^(search|tel)$/.test(h.type)||(c=/^(url|email)$/.test(h.type)?h.checkValidity&&h.checkValidity()===false:h.value!=M)}N[a[b]]=!!c}return N}("search tel url email datetime date month week time datetime-local number range color".split(" "))}
var f={},l=e.documentElement,E=e.createElement("modernizr"),j=E.style,h=e.createElement("input"),M=":)",O=Object.prototype.toString,q=" -webkit- -moz- -o- -ms- -khtml- ".split(" "),F="Webkit Moz O ms Khtml".split(" "),v={svg:"http://www.w3.org/2000/svg"},d={},N={},L={},P=[],w,Q=function(a){var b=document.createElement("style"),c=e.createElement("div");b.textContent=a+"{#modernizr{height:3px}}";(e.head||e.getElementsByTagName("head")[0]).appendChild(b);c.id="modernizr";l.appendChild(c);a=c.offsetHeight===3;b.parentNode.removeChild(b);c.parentNode.removeChild(c);return!!a},o=function(){var a={select:"input",change:"input",submit:"form",reset:"form",error:"img",load:"img",abort:"img"};return function(b,c){c=c||document.createElement(a[b]||"div");b="on"+b;var k=b in c;if(!k){c.setAttribute||(c=document.createElement("div"));if(c.setAttribute&&c.removeAttribute){c.setAttribute(b,"");k=typeof c[b]=="function";if(typeof c[b]!="undefined")c[b]=u;c.removeAttribute(b)}}return k}}(),G={}.hasOwnProperty,R;R=typeof G!=="undefined"&&typeof G.call!=="undefined"?function(a,b){return G.call(a,b)}:function(a,b){return b in a&&typeof a.constructor.prototype[b]==="undefined"};d.flexbox=function(){var a=e.createElement("div"),b=e.createElement("div");(function(k,g,r,x){g+=":";k.style.cssText=(g+q.join(r+";"+g)).slice(0,-g.length)+(x||"")})(a,"display","box","width:42px;padding:0;");b.style.cssText=q.join("box-flex:1;")+"width:10px;";a.appendChild(b);l.appendChild(a);var c=b.offsetWidth===42;a.removeChild(b);l.removeChild(a);return c};d.canvas=function(){var a=e.createElement("canvas");return!!(a.getContext&&a.getContext("2d"))};d.canvastext=function(){return!!(f.canvas&&typeof e.createElement("canvas").getContext("2d").fillText=="function")};d.webgl=function(){var a=e.createElement("canvas");try{if(a.getContext("webgl"))return true}catch(b){}try{if(a.getContext("experimental-webgl"))return true}catch(c){}return false};d.touch=function(){return"ontouchstart"in i||Q("@media ("+q.join("touch-enabled),(")+"modernizr)")};d.geolocation=function(){return!!navigator.geolocation};d.postmessage=function(){return!!i.postMessage};d.websqldatabase=function(){return!!i.openDatabase};d.indexedDB=function(){for(var a=-1,b=F.length;++a<b;){var c=F[a].toLowerCase();if(i[c+"_indexedDB"]||i[c+"IndexedDB"])return true}return false};d.hashchange=function(){return o("hashchange",i)&&(document.documentMode===u||document.documentMode>7)};d.history=function(){return!!(i.history&&history.pushState)};d.draganddrop=function(){return o("drag")&&o("dragstart")&&o("dragenter")&&o("dragover")&&o("dragleave")&&o("dragend")&&o("drop")};d.websockets=function(){return"WebSocket"in i};d.rgba=function(){j.cssText="background-color:rgba(150,255,150,.5)";return s(j.backgroundColor,"rgba")};d.hsla=function(){j.cssText="background-color:hsla(120,40%,100%,.5)";return s(j.backgroundColor,"rgba")||s(j.backgroundColor,"hsla")};d.multiplebgs=function(){j.cssText="background:url(//:),url(//:),red url(//:)";return/(url\s*\(.*?){3}/.test(j.background)};d.backgroundsize=function(){return n("backgroundSize")};d.borderimage=function(){return n("borderImage")};d.borderradius=function(){return n("borderRadius","",function(a){return s(a,"orderRadius")})};d.boxshadow=function(){return n("boxShadow")};d.textshadow=function(){return e.createElement("div").style.textShadow===""};d.opacity=function(){var a=q.join("opacity:.5;")+"";j.cssText=a;return s(j.opacity,"0.5")};d.cssanimations=function(){return n("animationName")};d.csscolumns=function(){return n("columnCount")};d.cssgradients=function(){var a=("background-image:"+q.join("gradient(linear,left top,right bottom,from(#9f9),to(white));background-image:")+q.join("linear-gradient(left top,#9f9, white);background-image:")).slice(0,-17);j.cssText=a;return s(j.backgroundImage,"gradient")};d.cssreflections=function(){return n("boxReflect")};d.csstransforms=function(){return!!D(["transformProperty","WebkitTransform","MozTransform","OTransform","msTransform"])};d.csstransforms3d=function(){var a=!!D(["perspectiveProperty","WebkitPerspective","MozPerspective","OPerspective","msPerspective"]);if(a)a=Q("@media ("+q.join("transform-3d),(")+"modernizr)");return a};d.csstransitions=function(){return n("transitionProperty")};d.fontface=function(){var a,b=e.head||e.getElementsByTagName("head")[0]||l,c=e.createElement("style"),k=e.implementation||{hasFeature:function(){return false}};c.type="text/css";b.insertBefore(c,b.firstChild);a=c.sheet||c.styleSheet;b=k.hasFeature("CSS2","")?function(g){if(!(a&&g))return false;var r=false;try{a.insertRule(g,0);r=!/unknown/i.test(a.cssRules[0].cssText);a.deleteRule(a.cssRules.length-1)}catch(x){}return r}:function(g){if(!(a&&g))return false;a.cssText=g;return a.cssText.length!==0&&!/unknown/i.test(a.cssText)&&a.cssText.replace(/\r+|\n+/g,"").indexOf(g.split(" ")[0])===0};f._fontfaceready=function(g){g(f.fontface)};return b('@font-face { font-family: "font"; src: "font.ttf"; }')};d.video=function(){var a=e.createElement("video"),b=!!a.canPlayType;if(b){b=new Boolean(b);b.ogg=a.canPlayType('video/ogg; codecs="theora"');b.h264=a.canPlayType('video/mp4; codecs="avc1.42E01E"')||a.canPlayType('video/mp4; codecs="avc1.42E01E, mp4a.40.2"');b.webm=a.canPlayType('video/webm; codecs="vp8, vorbis"')}return b};d.audio=function(){var a=e.createElement("audio"),b=!!a.canPlayType;if(b){b=new Boolean(b);b.ogg=a.canPlayType('audio/ogg; codecs="vorbis"');b.mp3=a.canPlayType("audio/mpeg;");b.wav=a.canPlayType('audio/wav; codecs="1"');b.m4a=a.canPlayType("audio/x-m4a;")||a.canPlayType("audio/aac;")}return b};d.localstorage=function(){try{return"localStorage"in
i&&i.localStorage!==null}catch(a){return false}};d.sessionstorage=function(){try{return"sessionStorage"in i&&i.sessionStorage!==null}catch(a){return false}};d.webWorkers=function(){return!!i.Worker};d.applicationcache=function(){return!!i.applicationCache};d.svg=function(){return!!e.createElementNS&&!!e.createElementNS(v.svg,"svg").createSVGRect};d.inlinesvg=function(){var a=document.createElement("div");a.innerHTML="<svg/>";return(a.firstChild&&a.firstChild.namespaceURI)==v.svg};d.smil=function(){return!!e.createElementNS&&/SVG/.test(O.call(e.createElementNS(v.svg,"animate")))};d.svgclippaths=function(){return!!e.createElementNS&&/SVG/.test(O.call(e.createElementNS(v.svg,"clipPath")))};for(var H in d)if(R(d,H)){w=H.toLowerCase();f[w]=d[H]();P.push((f[w]?"":"no-")+w)}f.input||S();f.crosswindowmessaging=f.postmessage;f.historymanagement=f.history;f.addTest=function(a,b){a=a.toLowerCase();if(!f[a]){b=!!b();l.className+=" "+(b?"":"no-")+a;f[a]=b;return f}};j.cssText="";E=h=null;i.attachEvent&&function(){var a=e.createElement("div");a.innerHTML="<elem></elem>";return a.childNodes.length!==1}()&&function(a,b){function c(p){for(var m=-1;++m<r;)p.createElement(g[m])}function k(p,m){for(var I=p.length,t=-1,y,J=[];++t<I;){y=p[t];m=y.media||m;J.push(k(y.imports,m));J.push(y.cssText)}return J.join("")}var g="abbr|article|aside|audio|canvas|details|figcaption|figure|footer|header|hgroup|mark|meter|nav|output|progress|section|summary|time|video".split("|"),r=g.length,x=RegExp("<(/*)(abbr|article|aside|audio|canvas|details|figcaption|figure|footer|header|hgroup|mark|meter|nav|output|progress|section|summary|time|video)","gi"),T=RegExp("\\b(abbr|article|aside|audio|canvas|details|figcaption|figure|footer|header|hgroup|mark|meter|nav|output|progress|section|summary|time|video)\\b(?!.*[;}])","gi"),z=b.createDocumentFragment(),A=b.documentElement,K=A.firstChild,B=b.createElement("style"),C=b.createElement("body");B.media="all";c(b);c(z);a.attachEvent("onbeforeprint",function(){for(var p=-1;++p<r;)for(var m=b.getElementsByTagName(g[p]),I=m.length,t=-1;++t<I;)if(m[t].className.indexOf("iepp_")<0)m[t].className+=" iepp_"+g[p];K.insertBefore(B,K.firstChild);B.styleSheet.cssText=k(b.styleSheets,"all").replace(T,".iepp_$1");z.appendChild(b.body);A.appendChild(C);C.innerHTML=z.firstChild.innerHTML.replace(x,"<$1bdo")});a.attachEvent("onafterprint",function(){C.innerHTML="";A.removeChild(C);K.removeChild(B);A.appendChild(z.firstChild)})}(this,document);f._enableHTML5=true;f._version="1.6";l.className=l.className.replace(/\bno-js\b/,"")+" js";l.className+=" "+P.join(" ");return f}(this,this.document);

/* - selectivizr.js - */
// https://www.ife.no/portal_javascripts/selectivizr.js?original=1
(function(x){function K(a){return a.replace(L,o).replace(M,function(b,e,c){b=c.split(",");c=0;for(var g=b.length;c<g;c++){var j=N(b[c].replace(O,o).replace(P,o))+t,f=[];b[c]=j.replace(Q,function(d,k,l,i,h){if(k){if(f.length>0){d=f;var u;h=j.substring(0,h).replace(R,n);if(h==n||h.charAt(h.length-1)==t)h+="*";try{u=v(h)}catch(da){}if(u){h=0;for(l=u.length;h<l;h++){i=u[h];for(var y=i.className,z=0,S=d.length;z<S;z++){var q=d[z];if(!RegExp("(^|\\s)"+q.className+"(\\s|$)").test(i.className))if(q.b&&(q.b===true||q.b(i)===true))y=A(y,q.className,true)}i.className=y}}f=[]}return k}else{if(k=l?T(l):!B||B.test(i)?{className:C(i),b:true}:null){f.push(k);return"."+k.className}return d}})}return e+b.join(",")})}function T(a){var b=true,e=C(a.slice(1)),c=a.substring(0,5)==":not(",g,j;if(c)a=a.slice(5,-1);var f=a.indexOf("(");if(f>-1)a=a.substring(0,f);if(a.charAt(0)==":")switch(a.slice(1)){case "root":b=function(d){return c?d!=D:d==D};break;case "target":if(p==8){b=function(d){function k(){var l=location.hash,i=l.slice(1);return c?l==""||d.id!=i:l!=""&&d.id==i}x.attachEvent("onhashchange",function(){r(d,e,k())});return k()};break}return false;case "checked":b=function(d){U.test(d.type)&&d.attachEvent("onpropertychange",function(){event.propertyName=="checked"&&r(d,e,d.checked!==c)});return d.checked!==c};break;case "disabled":c=!c;case "enabled":b=function(d){if(V.test(d.tagName)){d.attachEvent("onpropertychange",function(){event.propertyName=="$disabled"&&r(d,e,d.a===c)});w.push(d);d.a=d.disabled;return d.disabled===c}return a==":enabled"?c:!c};break;case "focus":g="focus";j="blur";case "hover":if(!g){g="mouseenter";j="mouseleave"}b=function(d){d.attachEvent("on"+(c?j:g),function(){r(d,e,true)});d.attachEvent("on"+(c?g:j),function(){r(d,e,false)});return c};break;default:if(!W.test(a))return false;break}return{className:e,b:b}}function C(a){return E+"-"+(p==6&&X?Y++:a.replace(Z,function(b){return b.charCodeAt(0)}))}function N(a){return a.replace(F,o).replace($,t)}function r(a,b,e){var c=a.className;b=A(c,b,e);if(b!=c){a.className=b;a.parentNode.className+=n}}function A(a,b,e){var c=RegExp("(^|\\s)"+b+"(\\s|$)"),g=c.test(a);return e?g?a:a+t+b:g?a.replace(c,o).replace(F,o):a}function G(a,b){if(/^https?:\/\//i.test(a))return b.substring(0,b.indexOf("/",8))==a.substring(0,a.indexOf("/",8))?a:null;if(a.charAt(0)=="/")return b.substring(0,b.indexOf("/",8))+a;var e=b.split("?")[0];if(a.charAt(0)!="?"&&e.charAt(e.length-1)!="/")e=e.substring(0,e.lastIndexOf("/")+1);return e+a}function H(a){if(a){s.open("GET",a,false);s.send();return(s.status==200?s.responseText:n).replace(aa,n).replace(ba,function(b,e,c){return H(G(c,a))})}return n}function ca(){var a,b;a=m.getElementsByTagName("BASE");for(var e=a.length>0?a[0].href:m.location.href,c=0;c<m.styleSheets.length;c++){b=m.styleSheets[c];if(b.href!=n)if(a=G(b.href,e))b.cssText=K(H(a))}w.length>0&&setInterval(function(){for(var g=0,j=w.length;g<j;g++){var f=w[g];if(f.disabled!==f.a)if(f.disabled){f.disabled=false;f.a=true;f.disabled=true}else f.a=f.disabled}},250)}if(!/*@cc_on!@*/true){var m=document,D=m.documentElement,s=function(){if(x.XMLHttpRequest)return new XMLHttpRequest;try{return new ActiveXObject("Microsoft.XMLHTTP")}catch(a){return null}}(),p=/MSIE ([\d])/.exec(navigator.userAgent)[1];if(!(m.compatMode!="CSS1Compat"||p<6||p>8||!s)){var I={NW:"*.Dom.select",DOMAssistant:"*.$",Prototype:"$$",YAHOO:"*.util.Selector.query",MooTools:"$$",Sizzle:"*",jQuery:"*",dojo:"*.query"},v,w=[],Y=0,X=true,E="slvzr",J=E+"DOMReady",aa=/(\/\*[^*]*\*+([^\/][^*]*\*+)*\/)\s*/g,ba=/@import\s*url\(\s*(["'])?(.*?)\1\s*\)[\w\W]*?;/g,W=/^:(empty|(first|last|only|nth(-last)?)-(child|of-type))$/,L=/:(:first-(?:line|letter))/g,M=/(^|})\s*([^\{]*?[\[:][^{]+)/g,Q=/([ +~>])|(:[a-z-]+(?:\(.*?\)+)?)|(\[.*?\])/g,R=/(:not\()?:(hover|enabled|disabled|focus|checked|target|active|visited|first-line|first-letter)\)?/g,Z=/[^\w-]/g,V=/^(INPUT|SELECT|TEXTAREA|BUTTON)$/,U=/^(checkbox|radio)$/,B=p==8?/[\$\^]=(['"])\1/:p==7?/[\$\^*]=(['"])\1/:null,O=/([(\[+~])\s+/g,P=/\s+([)\]+~])/g,$=/\s+/g,F=/^\s*((?:[\S\s]*\S)?)\s*$/,n="",t=" ",o="$1";m.write("<script id="+J+" defer src='//:'><\/script>");m.getElementById(J).onreadystatechange=function(){if(this.readyState=="complete"){a:{var a;for(var b in I)if(x[b]&&(a=eval(I[b].replace("*",b)))){v=a;break a}v=false}if(v){ca();this.parentNode.removeChild(this)}}}}}})(this);

/* - main.js - */
// https://www.ife.no/portal_javascripts/main.js?original=1
(function($){$(function(){var heightEqualizer;$(document).click(function(e){if($(e.target).closest('div.navSubLevel').length>0){return true}
else{if($('div.navSubLevel').is(':visible')){$('div.navSubLevel').slideUp(200)}}});$('#portaltab-subjectareas > a').click(function(e){e.stopPropagation();$(this).siblings('.navSubLevel').slideToggle(200);return false});heightEqualizer=function(elems,base_height){var elemHeight,outerStuff;if(elems.eq(0).hasClass('equalized')){return false}
if(base_height===0){base_height=Math.max.apply(null,elems.map(function(){return $(this).height()}).get());elems.height(base_height);elems.each(function(){if($(this).find('img').length===0){$(this).addClass('equalized')}})}
else{$(elems).each(function(){outerStuff=$(this).outerHeight(true)-$(this).height();elemHeight=base_height-outerStuff;$(this).height(elemHeight)})}
return base_height};$('#frontpage-columns').each(function(){heightEqualizer($(this).find('.fpBlock'),0)})})}(jQuery));$(window).load(function(){$.get(navigation_root_url+'/@@dropdown', function(data){$("#portaltab-subjectareas > a").after(data)});var subnav=$(".portletNavigationTree .navTreeLevel0");if(subnav.length){$("#viewlet-above-content").append("<div id='subnav'></div>")
subnav.clone().appendTo("#subnav")}
$("#portal-topactions").clone().prependTo("#portal-footer")
var filter=$(".portletMainSubjectsFilter .navTreeLevel0")
if(filter.length){var $_GET={};document.location.search.replace(/\??(?:([^=]+)=([^&]*)&?)/g, function(){
function decode(s){return decodeURIComponent(s.split("+").join(" "))}
$_GET[decode(arguments[1])]=decode(arguments[2])});if("mainsubject" in $_GET){$("#viewlet-above-content").append("<div id='subjectfilter'><button type='button'>Filter</button></div>")}
else{$("#viewlet-above-content").append("<div id='subjectfilter' class='closed'><button type='button'>Filter</button></div>")}
filter.clone().appendTo("#subjectfilter");$('#subjectfilter button').click(function(){$('#subjectfilter').toggleClass('closed')})}
$('.plone-navbar-toggle').click(function(){$('#portal-globalnav').toggleClass('expand');$('#portal-searchbox').toggleClass('expand');$('.plone-navbar-toggle').toggleClass('selected')})});
