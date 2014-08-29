// -- Adobe GoLive JavaScript Library
// -- Global Functions
CSAg = window.navigator.userAgent; CSBVers = parseInt(CSAg.charAt(CSAg.indexOf("/")+1),10);
CSIsW3CDOM = ((document.getElementById) && !(IsIE()&&CSBVers<6)) ? true : false;
function IsIE() { return CSAg.indexOf("MSIE") > 0;}
function CSIEStyl(s) { return document.all.tags("div")[s].style; }
function CSNSStyl(s) { if (CSIsW3CDOM) return document.getElementById(s).style; else return CSFindElement(s,0);  }
CSIImg=false;
function CSInitImgID() {if (!CSIImg && document.images) { for (var i=0; i<document.images.length; i++) { if (!document.images[i].id) document.images[i].id=document.images[i].name; } CSIImg = true;}}
function CSFindElement(n,ly) { if (CSBVers<4) return document[n];
	if (CSIsW3CDOM) {CSInitImgID();return(document.getElementById(n));}
	var curDoc = ly?ly.document:document; var elem = curDoc[n];
	if (!elem) {for (var i=0;i<curDoc.layers.length;i++) {elem=CSFindElement(n,curDoc.layers[i]); if (elem) return elem; }}
	return elem;
}
function CSGetImage(n) {if(document.images) {return ((!IsIE()&&CSBVers<5)?CSFindElement(n,0):document.images[n]);} else {return null;}}
CSDInit=false;
function CSIDOM() { if (CSDInit)return; CSDInit=true; if(document.getElementsByTagName) {var n = document.getElementsByTagName('DIV'); for (var i=0;i<n.length;i++) {CSICSS2Prop(n[i].id);}}}
function CSICSS2Prop(id) { var n = document.getElementsByTagName('STYLE');for (var i=0;i<n.length;i++) { var cn = n[i].childNodes; for (var j=0;j<cn.length;j++) { CSSetCSS2Props(CSFetchStyle(cn[j].data, id),id); }}}
function CSFetchStyle(sc, id) {
	var s=sc; while(s.indexOf("#")!=-1) { s=s.substring(s.indexOf("#")+1,sc.length); if (s.substring(0,s.indexOf("{")).toUpperCase().indexOf(id.toUpperCase())!=-1) return(s.substring(s.indexOf("{")+1,s.indexOf("}")));}
	return "";
}
function CSGetStyleAttrValue (si, id) {
	var s=si.toUpperCase();
	var myID=id.toUpperCase()+":";
	var id1=s.indexOf(myID);
	if (id1==-1) return "";
	s=s.substring(id1+myID.length+1,si.length);
	var id2=s.indexOf(";");
	return ((id2==-1)?s:s.substring(0,id2));
}
function CSSetCSS2Props(si, id) {
	var el=document.getElementById(id);
	if (el==null) return;
	var style=document.getElementById(id).style;
	if (style) {
		if (style.left=="") style.left=CSGetStyleAttrValue(si,"left");
		if (style.top=="") style.top=CSGetStyleAttrValue(si,"top");
		if (style.width=="") style.width=CSGetStyleAttrValue(si,"width");
		if (style.height=="") style.height=CSGetStyleAttrValue(si,"height");
		if (style.visibility=="") style.visibility=CSGetStyleAttrValue(si,"visibility");
		if (style.zIndex=="") style.zIndex=CSGetStyleAttrValue(si,"z-index");
	}
}
function CSSetStylePos(s,d,p) {
	if (CSIsW3CDOM)d==0?document.getElementById(s).style.left=p+"px":document.getElementById(s).style.top=p+"px";
	else if(IsIE())(d==0)?CSIEStyl(s).posLeft=p:CSIEStyl(s).posTop=p;
	else (d==0)?CSNSStyl(s).left=p:CSNSStyl(s).top=p;
}
function CSGetStylePos(s,d) {
	if (CSIsW3CDOM){CSIDOM();return parseInt((d==0)?document.getElementById(s).style.left:document.getElementById(s).style.top);}
	else if (IsIE()) {CSIEWinInit();return(d==0)?CSIEStyl(s).posLeft:CSIEStyl(s).posTop;}
	else {return (d==0)?CSNSStyl(s).left:CSNSStyl(s).top;}
}
CSIEWInit=false;
function CSIEWinInit() { if(CSIEWInit==true) return; else CSIEWInit=true; if (IsIE()&&(CSAg.indexOf("Win")!=-1)&&CSBVers==4) { var i=0; var lyr=document.all.tags("div")[i++]; while(lyr) {lyr.style.posLeft=lyr.offsetLeft; lyr.style.posTop=lyr.offsetTop; lyr=document.all.tags("div")[i++];}}}
CSLoopIsRunning = false; CSFctArray = new Array; CSTimeoutID = null;
function CSLoop() {	
	CSLoopIsRunning = false;
	for (i=0;i<CSFctArray.length;i++) {
		var curFct = CSFctArray[i];
		if (curFct)	{
			if (curFct.DoFunction(curFct)) { CSLoopIsRunning = true; curFct.counter++; }
			else CSFctArray[i] = 0;
		}
	}
	if (CSLoopIsRunning) CSTimeoutID = setTimeout("CSLoop()", 1);
}
function CSStartFunction(fct,data) {
	if (!CSLoopIsRunning) { CSFctArray = 0; CSFctArray = new Array; }
	var fctInfo = new Object;
	fctInfo.DoFunction = fct; fctInfo.counter = 0; fctInfo.data = data;
	CSFctArray[CSFctArray.length] = fctInfo; 
	if (!CSLoopIsRunning) CSLoop();
}
function CSStopFunction(sceneName) {
	var i;
	for (i=0;i<CSFctArray.length;i++) {
		var curFct = CSFctArray[i];
		if (curFct){ if (curFct.data.name == sceneName){ CSFctArray[i] = 0; return; } }
	}
}
function CSStopComplete() {
	if (CSTimeoutID == null) return;
	clearTimeout (CSTimeoutID); CSLoopIsRunning = false; CSTimeoutID = null;
}
function CSMoveLoop(fInf) {
	var ticks = 60 * (((new Date()).getTime()) - fInf.data.startTime)/1000;
	var f = ticks/fInf.data.ticks;
	if (f < 1) { CSSetStylePos(fInf.data.layer,0,fInf.data.start[0] * (1-f) + fInf.data.end[0] * f);
		CSSetStylePos(fInf.data.layer,1,fInf.data.start[1] * (1-f) + fInf.data.end[1] * f); return true; }
	else { CSSetStylePos(fInf.data.layer,0,fInf.data.end[0]);
		CSSetStylePos(fInf.data.layer,1,fInf.data.end[1]); }
	return false;
}
function CSSlideObj (layer,start,end,ticks,startTime) {
	this.layer=layer;this.start=start;this.end=end;this.ticks=ticks;this.startTime=startTime;
}
function CSSlideLayer(l,pos,anim,ticks) {
	var x = pos[0]; var y = pos[1];
	if (l == '') return;
	if (!anim) { CSSetStylePos(l,0,x); CSSetStylePos(l,1,y); }
	else {  var fctData = new CSSlideObj(l,new Array(CSGetStylePos(l,0),CSGetStylePos(l,1)),new Array(x,y),ticks,(new Date()).getTime()); CSStartFunction(CSMoveLoop,fctData); }
}
function CSSetStyleVis(s,v) {
	if (CSIsW3CDOM){CSIDOM();document.getElementById(s).style.visibility=(v==0)?"hidden":"visible";}
	else if(IsIE())CSIEStyl(s).visibility=(v==0)?"hidden":"visible";
	else CSNSStyl(s).visibility=(v==0)?'hide':'show';
}
function CSGetStyleVis(s) {
	if (CSIsW3CDOM) {CSIDOM();return(document.getElementById(s).style.visibility=="hidden")?0:1;}
	else if(IsIE())return(CSIEStyl(s).visibility=="hidden")?0:1;
	else return(CSNSStyl(s).visibility=='hide')?0:1;
}
function CSGetLayerClip (el) {
	CSIDOM();
	if (el.isIE) return (new CSRect(0,0,el.offsetWidth,el.offsetHeight));
	else if (CSBVers>=5) return (new CSRect(0,0,parseInt(el.style.width),parseInt(el.style.height)));
	else return (new CSRect(el.clip.left,el.clip.top,el.clip.width,el.clip.height));
}
function CSSetLayerClip (el,clipRect) {
    var l,t,r,b;
    l=clipRect.left; t=clipRect.top; r=l+clipRect.width; b=t+clipRect.height;
    if(el.isIE) { el.style.clip = "rect("+ t + " " + r + " " + b + " " + l + ")"; }
    else if (CSBVers>=5) el.style.clip = "rect("+ t + "px, " + r + "px, " + b + "px, " + l + "px)";
    else { el.clip.left=l; el.clip.top=t; el.clip.width=clipRect.width; el.clip.height=clipRect.height; }
	CSSetStyleVis(el.layer);
}
function CSRect (left,top,width,height) {
this.left=left; this.top=top; this.width=width; this.height=height;
}
function CSCreateTransElement (layer, steps) {
	var el;
	if (IsIE()) el=document.all.tags("div")[layer];
	else if (CSBVers>=5) el=document.getElementById(layer);
	else el=CSNSStyl(layer);
	if (el==null) return null;
	if (el.locked && (el.locked == true)) return null;
	el.isIE=IsIE();
	el.clipRect=CSGetLayerClip(el);
	if (el.clipRect==null) return null;
	el.maxValue=steps;
	if (el.maxValue<=0) el.maxValue=30;
	el.modus=""; el.layer=layer;
	el.width=el.clipRect.width; el.height=el.clipRect.height;
	el.locked = true;
	return el;
}
function CSDisposeTransElement (el) { el.locked = false; }
CSStateArray = new Object;
CSCookieArray = new Object;
CSCookieValArray = new Object;
function CSWriteCookie(action) {
	var name   = "DFT" + action[1];
	var hrs    = action[2];
	var path   = action[3];
	var domain = action[4];
	var secure = action[5];	
	var exp    = new Date((new Date()).getTime() + hrs * 3600000);	
	var cookieVal = "";
	for(var prop in CSCookieArray) {
		if(("DFT" + CSCookieArray[prop]) == name) {
			if(cookieVal != "") cookieVal += "&";
			cookieVal += prop + ":" + escape(CSStateArray[prop]);
		}
	}
	if(hrs != 0)
		cookieVal += "; expires=" + exp.toGMTString();
	if(path != "")
		cookieVal += "; path=" + path;
	if(domain != "")
		cookieVal += "; domain=" + domain;
	if(secure == true)
		cookieVal += "; secure";
	document.cookie = name + '=' + cookieVal;
}
function CSReadCookie(action) {
	var name    = "DFT" + action[1];
	var cookies = document.cookie;
	if(cookies == "") return;
	var start = cookies.indexOf(name);
	if(start == -1) return;
	start += name.length + 1;
	var end = cookies.indexOf(";", start);
	if(end == -1) end = cookies.length;
	var cookieVal = cookies.substring(start, end);
	var arr = cookieVal.split('&');
	for(var i = 0; i < arr.length; i++) {
		var a = arr[i].split(':');
		CSStateArray[a[0]] = unescape(a[1]);
	}	
}
function CSDefineState(action) {
	CSCookieArray[action[1]] = action[3]; 
}
function CSSetState(action) {
	CSStateArray[action[1]] = action[2];
}
function CSInitState(action) {
	if(typeof(CSStateArray[action[1]]) == "undefined")
		CSStateArray[action[1]] = action[2];
}
function CSCheckState(action) {
	var obj1 = CSStateArray[action[1]];
	var obj2 = action[2];
	if(typeof(obj1) == "object") {
		for(var i=0;i<obj1.length;i++) {
			if(obj1[i] != obj2[i])
				return false;
			}
		return true;
		}
	var res;
	var op = action[3];
		     if(op == "==") res = (CSStateArray[action[1]] == action[2]);	
		else if(op == "!=") res = (CSStateArray[action[1]] != action[2]);	
		else if(op == ">" ) res = (CSStateArray[action[1]] >  action[2]);	
		else if(op == ">=") res = (CSStateArray[action[1]] >= action[2]);	
		else if(op == "<" ) res = (CSStateArray[action[1]] <  action[2]);	
		else if(op == "<=") res = (CSStateArray[action[1]] <= action[2]);	
	return res;
}
function CSScriptInit() {
if(typeof(skipPage) != "undefined") { if(skipPage) return; }
idxArray = new Array;
for(var i=0;i<CSInit.length;i++)
	idxArray[i] = i;
CSAction2(CSInit, idxArray);
}
function CSScriptExit() {
idxArray = new Array;
for(var i=0;i<CSExit.length;i++)
	idxArray[i] = i;
CSAction2(CSExit, idxArray);
}
CSInit = new Array;
CSExit = new Array;
CSStopExecution=false;
function CSAction(array) {return CSAction2(CSAct, array);}
function CSAction2(fct, array) { 
	var result;
	for (var i=0;i<array.length;i++) {
		if(CSStopExecution) return false; 
		var aa = fct[array[i]];
		if (aa == null) return false;
		var ta = new Array;
		for(var j=1;j<aa.length;j++) {
			if((aa[j]!=null)&&(typeof(aa[j])=="object")&&(aa[j].length==2)){
				if(aa[j][0]=="VAR"){ta[j]=CSStateArray[aa[j][1]];}
				else{if(aa[j][0]=="ACT"){ta[j]=CSAction(new Array(new String(aa[j][1])));}
				else ta[j]=aa[j];}
			} else ta[j]=aa[j];
		}			
		result=aa[0](ta);
	}
	return result;
}
CSAct = new Object;
CSIm=new Object();
function CSIShow(n,i) {
	if (document.images) {
		if (CSIm[n]) {
			var img=CSGetImage(n);
			if (img&&typeof(CSIm[n][i].src)!="undefined") {img.src=CSIm[n][i].src;}
			if(i!=0) self.status=CSIm[n][3]; else self.status=" ";
			return true;
		}
	}
	return false;
}
function CSILoad(action) {
	im=action[1];
	if (document.images) {
		CSIm[im]=new Object();
		for (var i=2;i<5;i++) {
			if (action[i]!='') {CSIm[im][i-2]=new Image(); CSIm[im][i-2].src=action[i];}
			else CSIm[im][i-2]=0;
		}
		CSIm[im][3] = action[5];
	}
}
function newImage(arg) {
	if (document.images) {
		rslt = new Image();
		rslt.src = arg;
		return rslt;
	}
}
userAgent = window.navigator.userAgent;
browserVers = parseInt(userAgent.charAt(userAgent.indexOf("/")+1),10);
mustInitImg = true;
function initImgID() {di = document.images; if (mustInitImg && di) { for (var i=0; i<di.length; i++) { if (!di[i].id) di[i].id=di[i].name; } mustInitImg = false;}}
function findElement(n,ly) {
	d = document;
	if (browserVers < 4)		return d[n];
	if ((browserVers >= 6) && (d.getElementById)) {initImgID; return(d.getElementById(n))}; 
	var cd = ly ? ly.document : d;
	var elem = cd[n];
	if (!elem) {
		for (var i=0;i<cd.layers.length;i++) {
			elem = findElement(n,cd.layers[i]);
			if (elem) return elem;
		}
	}
	return elem;
}
function changeImages() {
	d = document;
	if (d.images) {
		var img;
		for (var i=0; i<changeImages.arguments.length; i+=2) {
			img = null;
			if (d.layers) {img = findElement(changeImages.arguments[i],0);}
			else {img = d.images[changeImages.arguments[i]];}
			if (img) {img.src = changeImages.arguments[i+1];}
		}
	}
}
function CSClickReturn () {
	var bAgent = window.navigator.userAgent; 
	var bAppName = window.navigator.appName;
	if ((bAppName.indexOf("Explorer") >= 0) && (bAgent.indexOf("Mozilla/3") >= 0) && (bAgent.indexOf("Mac") >= 0))
		return true; // dont follow link
	else return false; // dont follow link
}
function CSButtonReturn () { return !CSClickReturn(); }
function CSBrowserSwitch(action) {
	var bAgent	= window.navigator.userAgent;
	var bAppName	= window.navigator.appName;
	var isNS		= (bAppName.indexOf("Netscape") >= 0);
	var isIE		= (bAppName.indexOf("Explorer") >= 0);
	var isWin		= (bAgent.indexOf("Win") >= 0); 
	var isMac		= (bAgent.indexOf("Mac") >= 0); 
	var vers		= 0;
	var versIdx	= (bAgent.indexOf("Mozilla/"));
	if(versIdx >= 0)
		{
		var sstr	= bAgent.substring(versIdx + 8, versIdx + 9);
		vers		= parseInt(sstr) - 2;
		}
	var url		= action[1];
	var platform	= action[2];
	var versVec;
	if(platform)
		{
		if(isNS && isMac) versVec = action[3];
		if(isIE && isMac) versVec = action[5];
		if(isNS && isWin) versVec = action[4];
		if(isIE && isWin) versVec = action[6];
		}
	else
		{
		if(isNS) versVec = action[3];
		if(isIE) versVec = action[4];
		}
	if(vers > (versVec.length-1))
		vers = versVec.length-1;
	if(versVec[vers] == 0)
		{
		location			= url;
		CSStopExecution	= true;	
		}
}
function CSURLPopupShow(formName, popupName, target) {
	var form  = (!IsIE()&&CSBVers>=5)?document.forms[formName]:CSFindElement(formName);
	var popup = form.elements[popupName];
	window.open(popup.options[popup.selectedIndex].value, target);
	popup.selectedIndex = 0;
}
function CSSetStyleDepth(s,depth) {
	if (CSIsW3CDOM)document.getElementById(s).style.zIndex=depth;
	else if (IsIE())CSIEStyl(s).zIndex=depth;
	else CSNSStyl(s).zIndex=depth;
}
function CSGetStyleDepth(s) {
	if (CSIsW3CDOM){CSIDOM();return document.getElementById(s).style.zIndex;}
	else if (IsIE())return (CSIEStyl(s).zIndex);
	else return (CSNSStyl(s).zIndex);
}
CSSeqArray = new Array;
function CSSeqActionFct(seq,loopCount,continueLoop) {
	if ((seq.loop < 2) || ((loopCount % 2) != 0)) {
		for (var i=0;i<seq.actionCount;i++) {
			if (seq.actions[3*i + 1] <= seq.frame) {
				if ((loopCount > 1) && (seq.actions[3*i + 1] < seq.start)) continue;
				if (seq.actions[3*i + 2] < loopCount) {
					seq.actions[3*i + 2] = loopCount; CSLoopIsRunning = true;
					CSAction(new Array(seq.actions[3*i + 0])); continueLoop = true;
				}
			} else { continueLoop = true; break; }
		}
	} else {
		for (var i=seq.actionCount-1;i>=0;i--) {
			if (seq.actions[3*i + 1] > seq.frame) {
				if (seq.actions[3*i + 1] > seq.end) continue;
				if (seq.actions[3*i + 2] < loopCount) {
					seq.actions[3*i + 2] = loopCount; CSLoopIsRunning = true;
					CSAction(new Array(seq.actions[3*i + 0])); continueLoop = true;
				}
			} else { continueLoop = true; break; }
		}
	}
	return continueLoop;
}		
function CSSeqFunction(fctInfo)
{
	var seq = fctInfo.data; var oldFrame = seq.frame;
	var newTicks = (new Date()).getTime();
	seq.frame = Math.round((seq.fps * (newTicks - seq.startTicks)/1000.0) - 0.5);
	var continueLoop  = false; var loopCount = 1;
	
	if (seq.loop > 0) {
		continueLoop = true;
		if (seq.loop == 1) {
			var iv = (seq.end - seq.start);
			var f = Math.round(((seq.frame - seq.start) / iv) - 0.5);
			if (f < 0) f = 0;
			loopCount = f+1;
			seq.frame = seq.start + ((seq.frame - seq.start) % (seq.end - seq.start));
		} else {
			var iv = (seq.end - seq.start);
			var f = Math.round(((seq.frame - seq.start) / iv) - 0.5);
			if (f < 0) f = 0;
			loopCount = f+1;
			f = (seq.frame - seq.start) % (2 * iv);
			if (f > iv) f = 2*iv - f;
			seq.frame = seq.start + f;
		}
	}
	continueLoop = CSSeqActionFct(seq,loopCount,continueLoop);
	for (var i=0;i<seq.tracks.length;i++) {
		var track = seq.tracks[i]; var frameCount = 0; var lastCount = 0; var partCount = 0;
		var partIdx = track.parts.ticks.length;
		for (var k=0;k<track.parts.ticks.length;k++) {
			frameCount += track.parts.ticks[k];
			if (frameCount > seq.frame) { partIdx = k; partCount = seq.frame - lastCount; break; }
			lastCount = frameCount;
		}
		if (partIdx < track.parts.ticks.length) {
			var type=track.parts.moveType[partIdx];
			if(type==1) CSSetLinearPos (track, partIdx, partCount);
			else if(type==2) CSSetCurvePos (track, partIdx, partCount);
			else if(type==3) if (oldFrame != seq.frame) CSSetRandomPos (track, partIdx, partCount);
							 else { x = CSGetStylePos(track.layer,0); y = CSGetStylePos(track.layer,1); }
			CSSetStyleVis(track.layer,track.parts.visibilities[partIdx]);
			CSSetStyleDepth(track.layer,track.parts.depths[partIdx]);
			continueLoop = true;
		} else {
			var partIdx = track.parts.moveType.length-1;
			var posArray = track.parts.positions;
			var x = posArray[partIdx * 6 + 0]; var y = posArray[partIdx * 6 + 1];
			CSSetStylePos(track.layer,0,x); CSSetStylePos(track.layer,1,y);
			CSSetStyleVis(track.layer,track.parts.visibilities[partIdx]);
			CSSetStyleDepth(track.layer,track.parts.depths[partIdx]);
		}
	}
	return continueLoop;
}
function CSSetLinearPos (track, partIdx, partCount) {
	var curTicks = track.parts.ticks[partIdx];
	var pIdx1 = partIdx * 6; var pIdx2 = (partIdx+1) * 6;
	var posArray = track.parts.positions;
	var x = posArray[pIdx1 + 0]; var y = posArray[pIdx1 + 1];
	var x1,x2,y1,y2;
	var factor = partCount/curTicks;
	x1 = x; y1 = y;
	x2 = posArray[pIdx2 + 0]; y2 = posArray[pIdx2 + 1];
	x = x1 * (1-factor) + x2 * factor; y = y1 * (1-factor) + y2 * factor;
	CSSetStylePos(track.layer,0,x); CSSetStylePos(track.layer,1,y);
}
function CSSetCurvePos (track, partIdx, partCount) {
	var curTicks = track.parts.ticks[partIdx];
	var pIdx1 = partIdx * 6; var pIdx2 = (partIdx+1) * 6;
	var posArray = track.parts.positions;
	var x = posArray[pIdx1 + 0]; var y = posArray[pIdx1 + 1];
	var x1,x2,x3,x4,y1,y2,y3,y4;
	var factor = partCount/curTicks;
	var t = factor; var u = t * t; var v = u * t;
	var val1 = 3*(u-t) - v + 1; var val2 = 3*(v+t - 2*u); var val3 = 3*(u-v); var val4 = v;
	x1 = x; y1 = y; x2 = posArray[pIdx1 + 2]; y2 = posArray[pIdx1 + 3];
	x3 = posArray[pIdx1 + 4]; y3 = posArray[pIdx1 + 5];
	x4 = posArray[pIdx2 + 0]; y4 = posArray[pIdx2 + 1];
	x = x1 * val1 + x2 * val2 + x3 * val3 + x4 * val4;
	y = y1 * val1 + y2 * val2 + y3 * val3 + y4 * val4;
	CSSetStylePos(track.layer,0,x); CSSetStylePos(track.layer,1,y);
}
function CSSetRandomPos (track, partIdx, partCount) {
	var curTicks = track.parts.ticks[partIdx];
	var pIdx1 = partIdx * 6; var pIdx2 = (partIdx+1) * 6;
	var posArray = track.parts.positions;
	var x = posArray[pIdx1 + 0]; var y = posArray[pIdx1 + 1];
	var x1,x2,y1,y2;
	var factor = partCount/curTicks;
	x1 = x; y1 = y;
	x2 = posArray[pIdx2 + 0]; y2 = posArray[pIdx2 + 1];
	var factorx = Math.random(); var factory = Math.random();
	x = x1 * (1-factorx) + x2 * factorx; y = y1 * (1-factory) + y2 * factory;
	CSSetStylePos(track.layer,0,x); CSSetStylePos(track.layer,1,y);
}
function CSStartSeq(name) {
	var seq = CSGetScene(name); var date = new Date()
	seq.startTicks = date.getTime()
	for (var i=0;i<seq.actionCount;i++) seq.actions[3*i+2] = 0;
	CSStartFunction(CSSeqFunction,seq);
}
function CSSceneObj (name,fps,loop,start,end,frame,sTicks,numAct,acts,tracks) {
	this.name=name;this.fps=fps;this.loop=loop;this.start=start;this.end=end;
	this.frame=frame;this.startTicks=sTicks;this.actionCount=numAct;
	this.actions=acts;this.tracks=tracks;
}
function CSTrackObj (name,partIdx,partCount,parts) {
	this.layer=name;this.partIdx=partIdx;this.partCount=partCount;this.parts=parts;
}
function CSPartObj (ticks,pos,depths,vis,moveType) {
	this.ticks=ticks;this.positions=pos;this.depths=depths;this.visibilities=vis;
	this.moveType=moveType;
}
function CSGetScene (name) {
	for (i=0;i<CSSeqArray.length;i++) { var seq = CSSeqArray[i]; if (seq.name==name) return seq; }
	return 0;
}
function CSAutoStartScene(action) { CSStartSeq (action[1]); }
// -- Action Functions
//version 1.2
function CSBookmark(action){
	if (window.external&&navigator.appVersion.indexOf("Mac")==-1){
		js="javascript:"
		action[1]='"'+action[1]+'"'
		if(action[2]!=""){
		document.write("<a href='"+js+"window.external.AddFavorite(document.location,"+action[1]+")'>"+action[2]+"</a>");
		}else{
		document.write("<a href='"+js+"window.external.AddFavorite(document.location,"+action[1]+")'><img src='"+action[3]+"'border='0'></a>");
		}
	}
}
// MindPalette Cascading Menu Action v1.0 by Nate Baldwin, www.mindpalette.comm, copyright 2003...
function MPCascadeMenu(action) {
var thisForm = action[1];
var controlMenu = action[2];
var compValue = action[3];
var viewMenu = action[4];
if (document.forms[thisForm].elements[controlMenu].options[document.forms[thisForm].elements[controlMenu].selectedIndex].value == compValue) {
	document.forms[thisForm].elements[viewMenu].options.length = 0;
	var labelArray = new Array();
	var valueArray = new Array();
	var n = 0;
	for (i=5; i<44; i+=2) {
		if (action[i] != "" && action[i] != "#" && action[i] != "EmptyReference!" && action[i] != "Empty Reference!" && action[i] != null) {
			labelArray[n] = action[i];
			if (action[i+1] != "") {
				valueArray[n] = action[i+1];
				} else {
				valueArray[n] = action[i];
				}
			n++;
			}
		}
	for(i=0; i<labelArray.length; i++) {
		document.forms[thisForm].elements[viewMenu].options[i] = new Option(labelArray[i], valueArray[i]);
		}
	document.forms[thisForm].elements[viewMenu].options[0].selected = true;
	}
}
// Close Child Window Action 1.0 by Nate Baldwin, www.mindpalette.com - April 26, 2004
function MPCloseChildWindow(action) {
	if (action[1] != "" && typeof MPStoreOpenWin2 != "undefined") {
		if (typeof MPStoreOpenWin2[action[1]] == "object") {
			var thisPopup = MPStoreOpenWin2[action[1]];
			if (!thisPopup.closed) thisPopup.close();
			}
		}
	if (action[1] != "" && typeof MPStoreOpenWinL2 != "undefined") {
		if (typeof MPStoreOpenWinL2[action[1]] == "object") {
			var thisPopup = MPStoreOpenWinL2[action[1]];
			if (!thisPopup.closed) thisPopup.close();
			}
		}
	}
//-->
// Close Popup Redirect Action 1.01 by Nate Baldwin, www.mindpalette.com, copyright 2002...
var MP_CPRcounter = 0;
function MPCloseWin2(action) {
	MP_CPRcounter++;
	if (action[1] != "#" && action[1] != "<EmptyReference!)" && action[1] != "(Empty Reference)") self.opener.location = action[1];
	self.close();
	}
function MPFocusWin2() {
	if (MP_CPRcounter == 0) window.focus();
	}
window.onload = MPFocusWin2;
// Copy Field Value Action 1.02 by Nate Baldwin, mindpalette.com, copyright 2003...
function MPCopyFieldValue(action) {
	var thisForm = action[1];
	for (n=2; n<=21; n+=2) {
		if (action[n] != "") {
			var mainField = action[n];
			var copyField = action[n+1];
			var mainIsArray = false;
			mainFieldType = document.forms[thisForm].elements[mainField].type;
			if (!mainFieldType) mainFieldType = document.forms[thisForm].elements[mainField][0].type;
			copyFieldType = document.forms[thisForm].elements[copyField].type;
			if (!copyFieldType) copyFieldType = document.forms[thisForm].elements[copyField][0].type;
			with (document.forms[thisForm]) {
				var mainValue = "";
				if (mainFieldType == "text" || mainFieldType == "textarea")
					mainValue = elements[mainField].value;
				if (mainFieldType == "select-one")
					mainValue = elements[mainField].options[elements[mainField].options.selectedIndex].value;
				if (mainFieldType == "select-multiple") {
					var x=0;
					var selectArray = new Array();
					for (i=0; i<elements[mainField].length; i++) {
						if (elements[mainField].options[i].selected) {
							selectArray[x] = elements[mainField].options[i].value;
							x++;
							}
						}
					if (x > 0) {
						if (x == 1) mainValue = selectArray[0];
							else {
							mainValue = selectArray;
							mainIsArray = true;
							}
						}
					}
				if (mainFieldType == "checkbox") {
					if (elements[mainField].checked)
						mainValue = elements[mainField].value;
					}
				if (mainFieldType == "radio") {
					for (i=0; i<elements[mainField].length; i++) {
						if (elements[mainField][i].checked) mainValue = elements[mainField][i].value;
						}
					}
				if (copyFieldType == "text" || copyFieldType == "textarea") {
					if (!mainIsArray) elements[copyField].value = mainValue;
						else if (mainValue.length == 1) elements[copyField].value = mainValue[0]; 
						else {
						mainValueString = "";
						for (i=0; i<mainValue.length; i++) {
							if (mainValue[i] != "") {
								if (mainValueString != "") mainValueString += ", ";
								mainValueString += mainValue[i];
								}
							}
						elements[copyField].value = mainValueString;
						}
					}
				if (copyFieldType == "select-one") {
					if (!mainIsArray) mainValueString = mainValue;
						else mainValueString = mainValue[0];
					for (i=0; i<elements[copyField].length; i++) {
						if (elements[copyField].options[i].value == mainValueString)
							elements[copyField].selectedIndex = i;
						}
					}
				if (copyFieldType == "select-multiple") {
					for (i=0; i<elements[copyField].length; i++) {
						elements[copyField].options[i].selected = false;
						}
					if (!mainIsArray) {
						for (i=0; i<elements[copyField].length; i++) {
							if (elements[copyField].options[i].value == mainValue)
								elements[copyField].selectedIndex = i;
							}
						} else {
						for (x=0; x<mainValue.length; x++) {
							for (i=0; i<elements[copyField].length; i++) {
								if (elements[copyField].options[i].value == mainValue[x])
									elements[copyField].options[i].selected = true;
								}
							}
						}
					}
				if (copyFieldType == "checkbox") {
					if (!mainIsArray) {
						if (elements[copyField].value == mainValue)
							elements[copyField].checked = true;
						} else {
						for (i=0; i<mainValue.length; i++) {
							if (elements[copyField].value == mainValue[i])
								elements[copyField].checked = true;
							}
						}
					}
				if (copyFieldType == "radio") {
					if (!mainIsArray) {
						for (i=0; i<elements[copyField].length; i++) {
							if (elements[copyField][i].value == mainValue)
								elements[copyField][i].checked = true;
							}
						} else {
						var matchFound = false;
						for (x=0; x<mainValue.length; x++) {
							for (i=0; i<elements[copyField].length; i++) {
								if (elements[copyField][i].value == mainValue[x]) {
									if (matchFound == false) elements[copyField][i].checked = true;
									matchFound = true;
									}
								}
							}
						}
					}	
				} 
			}
		}
	}
// CSS Write Pro Action 3.1 for Adobe GoLive
// Copyright 2003 Scotland Software
// <http://www.scotlandsoftware.com/>
function isMSIE() {
	if ((navigator.appName.indexOf("Explorer") != -1) && (navigator.userAgent.indexOf("iCab") == -1) && (navigator.userAgent.indexOf("Opera") == -1) && (navigator.userAgent.indexOf("OmniWeb") == -1) && (parseInt(navigator.appVersion) >= 3)) {
		return true; }
	else {return false;}
}
function isNS() {
	if ((navigator.appName.indexOf("Netscape") != -1) && (navigator.userAgent.indexOf("iCab") == -1) && (navigator.userAgent.indexOf("Opera") == -1) && (navigator.userAgent.indexOf("OmniWeb") == -1) && (parseInt(navigator.appVersion) >= 4)) {
		return true; }
	else {return false;}
}
function isOpera() {
	if (((navigator.appName.indexOf("Opera") != -1) || (navigator.userAgent.indexOf("Opera") != -1)) && (navigator.userAgent.indexOf("iCab") == -1) && (navigator.userAgent.indexOf("OmniWeb") == -1) && (parseInt(navigator.appVersion) >= 4)) {
		return true; }
	else {return false;}
}
function isIcab() {
	if (((navigator.appName.indexOf("iCab") != -1) || (navigator.userAgent.indexOf("iCab") != -1)) && (navigator.userAgent.indexOf("Opera") == -1) && (navigator.userAgent.indexOf("OmniWeb") == -1) && (parseInt(navigator.appVersion) >= 4)) {
		return true; }
	else {return false;}
}
function isOmniweb() {
	if (((navigator.appName.indexOf("OmniWeb") != -1) || (navigator.userAgent.indexOf("OmniWeb") != -1)) && (navigator.userAgent.indexOf("Opera") == -1) && (navigator.userAgent.indexOf("iCab") == -1) && (parseInt(navigator.appVersion) >= 4)) {
		return true; }
	else {return false;}
}
function isSafari() {
	if (((navigator.userAgent.indexOf("Safari") != -1)) && (navigator.userAgent.indexOf("Opera") == -1) && (navigator.userAgent.indexOf("iCab") == -1) && (parseInt(navigator.appVersion) >= 4)) {
		return true; }
	else {return false;}
}
function isPC() {
	if ((navigator.userAgent.indexOf("Win") != -1) || (navigator.platform.indexOf("Win") != -1)) {
		return true; }
	else {return false;}
}
function isMac() {
	if ((navigator.appVersion.indexOf("Macintosh") != -1) || (navigator.platform.indexOf("Mac") != -1)) {
		return true; }
	else {return false;}
}
function doCSSWritePro(action) {
theStr="";
	if (isNS()) {
		if (isMac()) {
			theStr=action[1]; // Netscape 4 Mac
			if (parseInt(navigator.appVersion) >= 5) {
				theStr=action[2]; // Netscape 5 Mac
			}
			if (navigator.userAgent.indexOf("Netscape6") != -1) {
				theStr=action[3]; // Netscape 6 Mac
			}
			if (navigator.userAgent.indexOf("Netscape/7.0") != -1) {
				theStr=action[4]; // Netscape 7 Mac
			}
		}
		else if (isPC()) {
			theStr=action[5]; // Netscape 4 Win
			if (parseInt(navigator.appVersion) >= 5) {
				theStr=action[6]; // Netscape 5 Win
			}
			if (navigator.userAgent.indexOf("Netscape6") != -1) {
				theStr=action[7]; // Netscape 6 Win
			}
			if (navigator.userAgent.indexOf("Netscape/7.0") != -1) {
				theStr=action[8]; // Netscape 7 Win
			}
		}
	}
	if (isMSIE()) {
		if (isMac()) {
			theStr=action[9]; // Explorer 3 Mac
			if (parseInt(navigator.appVersion) >= 4) {
				theStr=action[10]; // Explorer 4 Mac
			}
			if ((navigator.userAgent.indexOf("MSIE 5") != -1) || (parseInt(navigator.appVersion) >= 5)) {
				theStr=action[11]; // Explorer 5+ Mac
			}
		}
		else if (isPC()) {
			theStr=action[12]; // Explorer 3 Win
			if (parseInt(navigator.appVersion) >= 4) {
				theStr=action[13]; // Explorer 4 Win
			}
			if ((navigator.userAgent.indexOf("MSIE 5") != -1) || (parseInt(navigator.appVersion) >= 5)) {
				theStr=action[14]; // Explorer 5 Win
			}
			if ((navigator.userAgent.indexOf("MSIE 6") != -1) || (parseInt(navigator.appVersion) >= 6)) {
				theStr=action[15]; // Explorer 6+ Win
			}
		}
	}
	if (isOpera()) {
		if (isMac()) {
			theStr=action[16]; // Opera Mac
		}
		else if (isPC()) {
			theStr=action[17]; // Opera Win
		}	
	}
	if (isIcab()) {
		if (isMac()) {
			theStr=action[18]; // iCab Mac
		}
	}
	if (isOmniweb()) {
		if (isMac()) {
			theStr=action[19]; // OmniWeb Mac (OS X)
		}
	}
	if (isSafari()) {
		if (isMac()) {
			theStr=action[20]; // Safari Mac (OS X)
		}
	}
	if (action[21]==true) {
		if (isNS()!=true && isMSIE()!=true && isOpera()!=true && isIcab()!=true && isOmniweb()!=true && isSafari()!=true && parseInt(navigator.appVersion)>=4) {
			theStr=action[22]; // Other
		}
	}
	// Write stylesheet reference
	if ((theStr!="") && (theStr!="(Empty Reference!)") && (theStr!="(EmptyReference!)")) {
		document.write("<LINK REL=\"stylesheet\" HREF=\""+theStr+"\" TYPE=\"text/css\">");
	}
} // end function
// DropDownPopUp 1.2.5 by Nate Baldwin, www.mindpalette.comm, copyright 2002
function MPDropPop(action) {
	for (i=7; i<14; i++) {
		if (action[i] == false) action[i] = "no"; else action[i] = "yes";
		}
	var posX = 0;
	var posY = 0;
	if (action[6] == true) {
		posX = ((screen.availWidth/2)-(action[4]/2));
		posY = ((screen.availHeight/2)-(action[5]/2));
		} else {
		posX = action[14];
		posY = action[15];
		}
	if (action[4] > (screen.availWidth - 10)) action[4] = (screen.availWidth - 10);
	if (action[5] > (screen.availHeight - 10)) action[5] = (screen.availHeight - 10);
	var thisForm = action[1];
	var thisMenu = action[2];	
	var thisURL = document.forms[thisForm].elements[thisMenu].options[document.forms[thisForm].elements[thisMenu].selectedIndex].value;
	if (thisURL) {
		if (thisURL != "" && thisURL != " " && thisURL != "#" && thisURL != "(EmptyReference!)" && thisURL != "(Empty Reference!)") {
			var windowOptions = "";
			windowOptions += "width=" + action[4];
			windowOptions += ",height=" + action[5];
			windowOptions += ",resizable=" + action[13];
			windowOptions += ",scrollbars=" + action[7];
			windowOptions += ",menubar=" + action[8];
			windowOptions += ",toolbar=" + action[11];
			windowOptions += ",directories=" + action[9];
			windowOptions += ",location=" + action[12];
			windowOptions += ",status=" + action[10];
			windowOptions += ",left=" + posX;
			windowOptions += ",top=" + posY;
			window.open(thisURL, action[3], windowOptions);
			}
		}
	}
function emailer(action) {
var mailStuff = "mailto:"+action[1]+"?Subject="+action[2]+"&body="+action[3];
window.location = mailStuff;
}
function focusFormField(action){
target=eval ("document."+action[1]+"."+action[2])
if (target){
target.focus()
}
}
// START FORM CALCULATOR 1.0.1 FUNCTIONS (www.mindpalette.com)
if (typeof MPFormCalcStoredVars == "undefined") var MPFormCalcStoredVars = new Array();
MPFormCalcStoredVars['val1var'] = 2000;
MPFormCalcStoredVars['val2var'] = 6000;
function MPFormCalcMain(action) {
	var formName = action[1];
	var field1 = MPFormCalcTrim(action[2]);
	var static1 = action[3];
	var var1 = action[4];
	var operation = action[5];
	var field2 = MPFormCalcTrim(action[6]);
	var static2 = action[7];
	var var2 = action[8];
	var roundOff = action[9];
	var forceDec = action[10];
	var absVal = action[11];
	var result_field = action[12];
	var result_id = action[13];
	var result_var = action[14];
	var round_type = action[15];
	var commas = action[16];
	var minSet = action[17];
	var minVal = parseFloat(action[18]);
	var maxSet = action[19];
	var maxVal = parseFloat(action[20]);
	var go = true;
	var numVal1 = 0;
	var numVal2 = 0;
	if (typeof document.forms[formName] == 'undefined') formName = 0;
	if (typeof document.forms[formName] == 'undefined') go = false;
	if (go) {
		var thisForm = document.forms[formName];
		numVal1 += MPFormCalcBuildNum(thisForm, field1, static1, var1);
		if (action[6] != '' || static2 != '' || var2 != '') {
			numVal2 += MPFormCalcBuildNum(thisForm, field2, static2, var2);
			numVal1 = MPFormCalcDoOp(numVal1, operation, numVal2);
			}
		if (minSet && numVal1 < minVal) numVal1 = minVal;
		if (maxSet && numVal1 > maxVal) numVal1 = maxVal;
		if (roundOff != '') numVal1 = MPFormCalcRound(numVal1, roundOff, round_type);
		if (absVal == true) numVal1 = Math.abs(numVal1);
		if (commas == true || forceDec > 0) numVal1 = MPFormCalcFormatString(numVal1, forceDec, commas);
		if (result_id != '') MPFormCalcDivText(result_id, numVal1);
		if (result_var != '') MPFormCalcStoredVars[result_var] = numVal1;
		if (result_field != '') thisForm.elements[result_field].value = numVal1;
		}
	}
function MPFormCalcVarType(thisField) {
	var fieldType = false;
	if (typeof thisField.type != 'undefined') fieldType = thisField.type;
	if (!fieldType) if (thisField[0].type != 'undefined') fieldType = 'group';
	if (!fieldType) fieldType = 'undefined';
	return fieldType;
	}
function MPFormCalcInArray(needle, haystack) {
	results = false;
	for (var n=0; n<haystack.length; n++) {
		if (haystack[n] == needle) {
			results = true;
			break;
			}
		}
	return results;
	}
function MPFormCalcTrim(inString) {
	var frontIndex = 0;
	var backIndex = inString.length - 1;
	var trimList = new Array(" ", "\t", "\n", "\r");
	while (MPFormCalcInArray(inString.charAt(frontIndex), trimList)) frontIndex++;
	while (MPFormCalcInArray(inString.charAt(backIndex), trimList)) backIndex--;
	return inString.substring(frontIndex, (backIndex + 1));
	}
function MPFormCalcTrimArray(thisArray) {
	var results = new Array();
	var count = 0;
	for (var n=0; n<thisArray.length; n++) {
		var thisVal = MPFormCalcTrim(thisArray[n]);
		if (thisVal != '') {
			results[count] = MPFormCalcTrim(thisArray[n]);
			count++;
			}
		}
	return results;
	}
function MPFormCalcDoOp(num1, operation, num2) {
	results = 0;
	if (operation == 1) results = num1 - num2;
		else if (operation == 2) results = num1 * num2;
		else if (operation == 3) results = num1 / num2;
		else if (operation == 4) results = num1 + ((num2 / 100) * num1);
		else if (operation == 5) results = num1 - ((num2 / 100) * num1);
		else results = num1 + num2;
	return results;
	}
function MPFormCalcGetNum(thisString) {
	thisString = thisString+"";
	var result = "";
	var regEx =  /^[0-9.]/;
	for (var n=0; n<thisString.length; n++) {
		var thisChar = thisString.charAt(n);
		if (regEx.test(thisChar)) result += thisChar;
		}
	if (result == '.' || result == 'NaN') result = 0;
	result = (result == '') ? 0 : parseFloat(result);
	if (result == "NaN") result = 0;
	return result;
	}
function MPFormCalcBuildNum(thisForm, fields, staticVal, varVal) {
	var numVal = 0;
	numVal += MPFormCalcGetNum(staticVal);
	if (varVal != '') {
		if (typeof MPFormCalcStoredVars[varVal] != "undefined") numVal += MPFormCalcGetNum(MPFormCalcStoredVars[varVal]);
		}
	if (fields != '') {
		fieldList = MPFormCalcTrimArray(fields.split(','));
		for (var n=0; n<fieldList.length; n++) {
			thisField = fieldList[n];
			if (thisField != '' && typeof thisForm.elements[thisField] != 'undefined') thisField = thisForm.elements[thisField];
			numVal += MPFormCalcFlatten(thisField);
			}
		}
	return numVal;
	}
function MPFormCalcFlatten(passedField) {
	var numVal = 0;
	fieldArray = (MPFormCalcVarType(passedField) != "group") ? new Array(passedField) : passedField;
	for (var n=0; n<fieldArray.length; n++) {
		var thisField = fieldArray[n];
		var fieldType = MPFormCalcVarType(thisField);
		if (fieldType == "text" || fieldType == "textarea" || fieldType == "hidden" || fieldType == "password") {
			numVal = MPFormCalcDoOp(numVal, 0, MPFormCalcGetNum(thisField.value));
			} else if (fieldType == "select-one")
			numVal = MPFormCalcDoOp(numVal, 0, MPFormCalcGetNum(thisField.options[thisField.options.selectedIndex].value));
			else if (fieldType == "select-multiple") {
			var thisSum = 0;
			for (var i=0; i<thisField.length; i++) {
				if (thisField.options[i].selected) {
					var selection = MPFormCalcGetNum(thisField.options[i].value);
					thisSum += selection;
					}
				}
			numVal = MPFormCalcDoOp(numVal, 0, thisSum);
			}
			else if (fieldType == "checkbox" || fieldType == "radio") {
			if (typeof thisField.length != "undefined") {
				var thisSum = 0;
				for (var i=0; i<thisField.length; i++) {
					if (thisField[i].checked) thisSum += MPFormCalcGetNum(thisField[i].value);
					}
				numVal = MPFormCalcDoOp(numVal, 0, thisSum);
				} else if (thisField.checked) {
				numVal = MPFormCalcDoOp(numVal, 0, MPFormCalcGetNum(thisField.value));
				}
			}
		}
	return numVal;
	}
function MPFormCalcRound(number, places, type) {
	var precision = (places != 0) ? Math.pow(10, Math.abs(places)) : 0;
	if (places > 0) number = number * precision;
		else if (places < 0) number = number / precision;
	if (type == 2) number = Math.ceil(number);
		else if (type == 1) number = Math.floor(number);
		else number = Math.round(number);
	if (places > 0) number = number / precision;
		else if (places < 0) number = number * precision;
	return number;
	}
function MPFormCalcSplitIntDec(number) {
	string = number+'';
	if (string.indexOf('.') > -1) var results = string.split('.');
		else results = new Array(string, '');
	return results;
	}
function MPFormCalcFormatString(number, force, commas) {
	var string = number+"";
	if (force > 0) {
		var temp = MPFormCalcSplitIntDec(string);
		var integer = temp[0];
		var decimal = temp[1];
		while (decimal.length < force) { decimal+="0" };
		string = integer+'.'+decimal;
		}
	if (commas) {
		var temp = MPFormCalcSplitIntDec(string);
		var integer = temp[0];
		var decimal = '.'+temp[1];
		var last = (integer.length - 1);
		if (last > 2) {
			var results = "";
			var n = last;
			while (n>-1) {
				x = 1;
				while (x<4 && n>-1) {
					results = string.charAt(n) + results;
					n--;
					x++;
					}
				if (n>-1) results = ','+results;
				}
			string = results+decimal;
			}
		}
	return string;
	}
function MPFormCalcDivText(div, text) {
	if (document.layers) {
		if (typeof eval("document."+div+".document") != "undefined") {
			with (eval("document."+div+".document")) {
				open();
				write("<"+"HTML"+"><"+"HEAD"+"><"+"/HEAD"+"><"+"BODY"+">"+text+"<"+"/BODY"+"><"+"/HTML"+">");
				close();
				}
			}
		} else if (document.all && typeof document.all[div] != "undefined") {
		document.all[div].innerHTML = text;
		} else if (document.getElementById && !document.all && typeof document.getElementById(div) != "undefined") {
		document.getElementById(div).innerHTML = text;
		}
	}
// END FORM CALCULATOR 1.0 FUNCTIONS
//-->
function doHourlyMessage(action){
// JavaScript by Matt Ridley <http://www.matt-ridley.com/> March 2000
// 
// Writes out a different message depending on what hour it is.
// 	- Can be customized to change font, size, style.
// 	- Can choose whether the message is linked to an URL of your choice.
// 	- Can choose whether a readout of time is included.
// 
// Dedicated to Jeri Ryan (Seven of Nine) <http://www.jerilynn.com/>
theStr="";
theDay = new Date();
theHour = theDay.getHours();
switch (theHour) {
case 0 : // between 12am and 1am
theStr=action[6]
break;
case 1 : // between 1am and 2am
theStr=action[7]
break;
case 2 : // between 2am and 3am
theStr=action[8]
break;
case 3 : // between 3am and 4am
theStr=action[9]
break;
case 4 : // between 4am and 5am
theStr=action[10]
break;
case 5 : // between 5am and 6am
theStr=action[11]
break;
case 6 : // between 6am and 7am
theStr=action[12]
break;
case 7 : // between 7am and 8am
theStr=action[13]
break;
case 8 : // between 8am and 9am
theStr=action[14]
break;
case 9 : // between 9am and 10am
theStr=action[15]
break;
case 10 : // between 10am and 11am
theStr=action[16]
break;
case 11 : // between 11am and 12pm
theStr=action[17]
break;
case 12 : // between 12pm and 1pm
theStr=action[18]
break;
case 13 : // between 1pm and 2pm
theStr=action[19]
break;
case 14 : // between 2pm and 3pm
theStr=action[20]
break;
case 15 : // between 3pm and 4pm
theStr=action[21]
break;
case 16 : // between 4pm and 5pm
theStr=action[22]
break;
case 17 : // between 5pm and 6pm
theStr=action[23]
break;
case 18 : // between 6pm and 7pm
theStr=action[24]
break;
case 19 : // between 7pm and 8pm
theStr=action[25]
break;
case 20 : // between 8pm and 9pm
theStr=action[26]
break;
case 21 : // between 9pm and 10pm
theStr=action[27]
break;
case 22 : // between 10pm and 11pm
theStr=action[28]
break;
case 23 : // between 11pm and 12am
theStr=action[29]
break;
} // end switch
theToken="pm";
if (theHour<=11) {
theToken="am";
} // end if
if (action[1]==true) {
theStr="The time is "+theHour+":"+theDay.getMinutes()+theToken+".<BR>"+theStr;
} // end if
if (action[3]==true) {
theStr="<I>"+theStr+"</I>";
} // end if
if (action[2]==true) {
theStr="<B>"+theStr+"</B>";
} // end if
if (action[4]==true) {
theStr="<A HREF="+action[5]+">"+theStr+"</A>";
} // end if
document.write(theStr);
} // end function
// Image Window Action 1.0 by Nate Baldwin, mindpalette.com, copyright 2002...
function MPImageMenu(action) {
var thisForm = action[1];
var thisMenu = action[2];
var thisImg = eval("document." + action[3]);
var thisImage = document.forms[thisForm].elements[thisMenu].options[document.forms[thisForm].elements[thisMenu].selectedIndex].value;
if (thisImage != "") {
thisImg.src = thisImage;
}
return (false)
}
// Image Sequence Action 1.2 by Nate Baldwin, www.mindpalette.com, copyright 2002
var MPThisFileName = "";
function MPImageSeq(action) {
var MPImageCountID = 0;
var reverse = action[2];
var endStop = action[3];
var imageList = new Array();
var seqImage = eval("document."+action[1]);
var currentImage = seqImage.src;
MPGetFileName(currentImage);
currentImage = MPThisFileName;
var n=0;
for (i=4; i<=23; i++) {
	if (action[i] && action[i] != "" && action[i] != "#" && action[i] != "(EmptyReference!)" && action[i] != "(Empty Reference!)") {
		imageList[n] = action[i];
		MPGetFileName(imageList[n]);
		var thisImage = MPThisFileName;
		if (thisImage == currentImage) MPImageCountID = n;
		n++;
		}
	}
if (reverse == false) {
	if (MPImageCountID >= imageList.length-1) {
		if (endStop == false) MPImageCountID = 0; else alert("End of Images\n("+imageList.length+" of "+imageList.length+")");
		} else {
		MPImageCountID++;
		}
	} else {
	if (MPImageCountID > 0) {
		MPImageCountID--;
		} else {
		if (endStop == false) MPImageCountID = imageList.length-1; else alert("Beginning of Images\n(1 of "+imageList.length+")");
		}
	}
seqImage.src = imageList[MPImageCountID];
}
function MPGetFileName(thisString) {
if (thisString.lastIndexOf('/') != -1) {
	var nameStart = thisString.lastIndexOf('/') + 1;
	var nameEnd = thisString.length;
	MPThisFileName = thisString.substring(nameStart, nameEnd);
	} else {
	MPThisFileName = thisString;
	}
}
// Detect Image Action 1.4 by Nate Baldwin, www.mindpalette.com, 2/9/03
function MPDetectImg() {
	var MPData = unescape(location.search);
	var MPImage = "";
	var MPImageX = "";
	var MPImageY = "";
	if (MPData != "") {
		var trimString = MPData.substring(1);
		if (trimString != "") {
			var allPairs = trimString.split("*");
			for (i=0; i<allPairs.length; i++) {
				var thisPair = allPairs[i];
				var splitPair = thisPair.split("=");
				if (splitPair[0] == "MPImage") MPImage = splitPair[1];
				if (splitPair[0] == "MPImageX") MPImageX = splitPair[1];
				if (splitPair[0] == "MPImageY") MPImageY = splitPair[1];
				}
			}
			if (MPImage != "" && MPImage != "#") {
			
				var newString = "";
				if (MPImage.indexOf("^") >= 0) {
					var testArray = MPImage.split("^");
					for (i=0; i<testArray.length; i++) {
						newString += ".";
						newString += testArray[i];
						}
					MPImage = newString.substr(1);
					}
				document.write("<img src=\""+MPImage+"\" alt=\"\"");
				}
			if (MPImageX != "" && MPImageY != "" && MPImageX != 0 && MPImageY != 0) document.write(" width=\""+MPImageX+"\" height=\""+MPImageY+"\"");
			if (MPImage != "" && MPImage != "#") document.write(" border=\"0\">");
		}
	window.focus();
	}
// Detect Title Action 1.4 by Nate Baldwin, www.mindpalette.com, 2/9/03
function MPDetectTitle() {
	var MPData2 = unescape(location.search);
	var MPTitle = "";
	if (MPData2 != "") {
		var trimString2 = MPData2.substring(1);
		if (trimString2 != "") {
			var allPairs2 = trimString2.split("*");
			for (i=0; i<allPairs2.length; i++) {
				var thisPair = allPairs2[i];
				var splitPair = thisPair.split("=");
				if (splitPair[0] == "MPTitle") MPTitle = splitPair[1];
				}
				if (MPTitle != "" && MPTitle != "#") document.write("<title>"+MPTitle+"</title>");
					else document.write("<title>Popup Window</title>");
		}
	}
	window.focus();
	}
// Auto Image Window Action 1.4.1 by Nate Baldwin, www.mindpalette.com, 12/2/03
function MPImageWindow(action) {
	for (i=10; i<17; i++) action[i] = (action[i] == false) ? "no" : "yes";
	if (action[7] > screen.availWidth) action[7] = screen.availWidth;
	if (action[8] > screen.availHeight) action[8] = screen.availHeight;
	var posX = 0;
	var posY = 0;
	if (action[9] == true) {
		posX = Math.round((screen.availWidth/2)-(action[7]/2));
		posY = Math.round((screen.availHeight/2)-(action[8]/2));
		} else {
		posX = action[17];
		posY = action[18];
		}
		
	var ImageURL = action[1];
	
	var newString = "";
	if (ImageURL.indexOf(".") >= 0) {
		var testArray = ImageURL.split(".");
		for (i=0; i<testArray.length; i++) {
			newString += "^";
			newString += testArray[i];
			}
		ImageURL = newString.substr(1);
		}
	
	var queryString = "*MPImage="+ImageURL+"*MPImageX="+action[2]+"*MPImageY="+action[3]+"*MPTitle="+action[4]+"*";
	var queryString = escape(queryString);
	var queryString = "?"+queryString;
	action[5] += queryString;
	var windowOptions = "";
	windowOptions += "width=" + action[7];
	windowOptions += ",height=" + action[8];
	windowOptions += ",resizable=" + action[16];
	windowOptions += ",scrollbars=" + action[10];
	windowOptions += ",menubar=" + action[11];
	windowOptions += ",toolbar=" + action[14];
	windowOptions += ",directories=" + action[12];
	windowOptions += ",location=" + action[15];
	windowOptions += ",status=" + action[13];
	windowOptions += ",left=" + posX;
	windowOptions += ",top=" + posY;
	window.open(action[5], action[6], windowOptions);
	}
//-->
// Link To Opener Action 1.0 by Nate Baldwin, www.mindpalette.com, copyright 2003
function MPLinkToOpener(action) {
	var linkurl = action[1];
	if (linkurl != "" && linkurl != "#" && linkurl != "(EmptyReference!)" && linkurl != "(Empty Reference!)") {
		var target = action[2];
		var winFocus = action[3];
		var winClose = action[4];
		var thisOpener = new Object();
		var frameFound = false;
		thisOpener = (!window.opener) ? self : window.opener;
		if (target != "") {
			for (i=0; i<thisOpener.parent.frames.length; i++) {
				if (thisOpener.parent.frames[i].name == target) {
					frameFound = true;
					break;
					}
				}
			}
		if (frameFound == true)
			thisOpener.parent.frames[target].location = linkurl;
			else thisOpener.location = linkurl;
		if (winFocus == true) self.focus(); else self.blur();
		if (winClose == true) self.close();
		}
	}
//-->
//MonthlyMessage Action v1.0  AardActions by Jim Aardvark. (Assistance by Nate Baldwin www.mindpalette.com) Copyright 2003. seanclayton@tinkers-inc.co.uk
function JAmonthMess(action) 
{  
var today = new Date(); 
var day = today.getDate(); 
var month= today.getMonth(); 
for ($n=0; $n<12; $n++) {
	if (month == $n) document.write(action[$n+1]);
	}
}
// Multi Preloader Action 1.0.2 by Nate Baldwin, www.mindpalette.com, copyright 2002
var MPMultiPreldCount1 = 0;
var MPMultiPreldArray1 = new Array();
function MPMultiPreload(action) {
	MPMultiPreldArray1[MPMultiPreldCount1] = new Array();
	for (i=1; i<=20; i++) {
		if (action[i] != "#" && action[i] != "(EmptyReference!)" && action[i] != "(Empty Reference)" && action[i] != "") {
			MPMultiPreldArray1[MPMultiPreldCount1][i] = new Image();
			MPMultiPreldArray1[MPMultiPreldCount1][i].src = action[i];
			}
		}
	MPMultiPreldCount1++;
	}
// Version 1.1. A Michael Ahgren action.
function CSRemoveIEbox(){
if (document.images&&navigator.userAgent.indexOf("MSIE")!=-1){
	for(i=0;i<document.links.length;i++){
		target=eval("document.links[i]")
		target.onfocus=blurHandler
		}
	}
}
function blurHandler(){
obj=window.event.srcElement
if(obj.tagName == "A"||obj.tagName=="AREA") obj.blur()
}
//NoSpam Action v1.1  AardActions by Jim Aardvark. Copyright 2003. seanclayton@tinkers-inc.co.uk
function JAnoSpam(action){
var user=action[1];
var domain=action[2];
var suffix=action[3];
var subject=action[4];
document.write('<a href="' + 'mailto:' + user + '@' + domain + suffix + '?subject=' + subject + '">' + user + '@' + domain + suffix + '</a>');
}
// OpenPopup 2.0 by Nate Baldwin, www.mindpalette.com - copyright 2003 (May 7, 2004)
if (typeof MPStoreOpenWin2 == "undefined") MPStoreOpenWin2 = new Array();
if (typeof MPStoreWinStngs2 == "undefined") {
	MPStoreWinStngs2 = new Object();
	MPStoreWinStngs2.url = "";
	MPStoreWinStngs2.name = "";
	MPStoreWinStngs2.options = "";
	MPStoreWinStngs2.focus = true;
	MPStoreWinStngs2.html = "";
	MPStoreWinStngs2.redirect = "";
	}
function MPOpenPopupWin2(action) {
	MPStoreWinStngs2.url = "";
	MPStoreWinStngs2.name = "";
	MPStoreWinStngs2.options = "";
	MPStoreWinStngs2.focus = true;
	MPStoreWinStngs2.html = "";
	MPStoreWinStngs2.redirect = "";
	var thisURL = (action[1] != "#" && action[1] != "(EmptyReference!)" && action[1] != "(Empty Reference!)") ? action[1] : "";
	var redirect = (action[31] != "#" && action[31] != "(EmptyReference!)" && action[31] != "(Empty Reference!)") ? action[31] : "";
	if (thisURL != "") {
		var urlType = (action[2] == 1) ? "image" : "page";
		MPStoreWinStngs2.focus = action[3];
		var regExpr = /[\w\S?]/;
		var validName = "";
		action[11] = (action[11] == true) ? "yes" : "no";
		var winName = (action[4] != "") ? action[4] : "PopUpWin";
		for (var n=0; n<winName.length; n++) {
			if (regExpr.test(winName.charAt(n))) validName += winName.charAt(n);
			}
		winName = (validName != "") ? validName : "PopUpWin";
		var width = (action[7] > screen.availWidth) ? screen.availWidth : action[7];
		var height = (action[9] > screen.availHeight) ? screen.availHeight : action[9];
		if (action[6] == true || action[8] == true) {
			width = screen.availWidth;
			posX = 0;
			}
		if (action[6] == true || action[10] == true) {
			height = screen.availHeight;
			posY = 0;
			}
		var posX = (action[13] != "") ? action[13] : 0;
		var posY = (action[15] != "") ? action[15] : 0;
		if (action[12] == true && width > 0 && height > 0) {
			posX = Math.round((screen.availWidth - width) / 2);
			posY = Math.round((screen.availHeight - height) / 2);
			} else {
			if (action[14] == 1 && width > 0) posX = (screen.availWidth - width - posX);
				else if (action[14] == 2) posX = Math.round((screen.availWidth - width) / 2);
			if (action[16] == 1 && height > 0) posY = (screen.availHeight - height - posY);
				else if (action[16] == 2) posY = Math.round((screen.availHeight - height) / 2);
			}
		for (i=17; i<=22; i++) {
			action[i] = (action[i] == true) ? "yes" : "no";
			}
		if (action[5] == true && typeof MPStoreOpenWin2[winName] != "undefined") {
			var oldWindow = MPStoreOpenWin2[winName];
			if (oldWindow && typeof oldWindow == "object") if (!oldWindow.closed) oldWindow.close();
			}
		var windowOptions = "";
		if (width > 0) windowOptions += "width=" + width + ",";
		if (height > 0) windowOptions += "height=" + height + ",";
		windowOptions += "resizable=" + action[11];
		windowOptions += ",scrollbars=" + action[17];
		windowOptions += ",menubar=" + action[18];
		windowOptions += ",toolbar=" + action[21];
		windowOptions += ",directories=" + action[19];
		windowOptions += ",location=" + action[22];
		windowOptions += ",status=" + action[20];
		windowOptions += ",left=" + posX;
		windowOptions += ",top=" + posY;
		if (urlType == "image") {
			var font = "Verdana, Arial, Helvetica, sans-serif";
			if (action[28] == 1) font = "Arial, Helvetica, Geneva, Swiss, SunSans-Regular, sans-serif";
				else if (action[28] == 2) font = "Helvetica, Geneva, Arial, SunSans-Regular, sans-serif";
				else if (action[28] == 3) font = "\"Times New Roman\", Times, Georgia, serif";
				else if (action[28] == 4) font = "\"Courier New\", Courier, Monaco, monospace";
			var textColor = (action[27] != "") ? action[27] : "black";
			var linkColor = (action[30] != "") ? action[30] : "#00417d";
			var textSize = (action[29] != "" && action[29] > 5) ? action[29]+"" : "12";
			var backGround = (action[24] != "") ? action[24] : "#ffffff";
			var thisHTML = "";
			thisHTML += '<'+'!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">'+"\n\n";
			thisHTML += '<'+'html>'+"\n\n";
			thisHTML += '<'+'head>'+"\n";
			thisHTML += '<'+'meta http-equiv="content-type" content="text/html;charset=iso-8859-1">'+"\n";
			if (action[23] != "") thisHTML += '<'+'title>'+action[23]+'<'+'/title>'+"\n";
			thisHTML += '<'+'style type="text/css" media="screen"><'+'!--'+"\n";
			thisHTML += 'body, div, td  { color: '+textColor+'; font-size: '+textSize+'px; font-family: '+font+'; background-color: '+backGround+'; margin: 0; padding: 0 }'+"\n";
			thisHTML += '.textpad  { margin: 0; padding: 4px 0 }'+"\n";
			if (action[25] == true) {
				thisHTML += 'a { color: '+linkColor+'; font-size: '+textSize+'px; text-decoration: none }'+"\n";
				thisHTML += 'a:link { color: '+linkColor+'; text-decoration: none }'+"\n";
				thisHTML += 'a:visited { color: '+linkColor+'; text-decoration: none }'+"\n";
				thisHTML += 'a:hover { color: '+linkColor+'; text-decoration: underline }'+"\n";
				}
			thisHTML += "--><"+"/style>\n<"+"/head>\n\n";
			thisHTML += '<'+'body bgcolor="'+backGround+'" leftmargin="0" marginheight="0" marginwidth="0" topmargin="0">'+"\n";
			if (action[26] != "") thisHTML += '<'+'div class="textpad" align="center">'+action[26]+'<'+'/div>'+"\n";
			thisHTML += '<'+'div align="center"><'+'img src="'+thisURL+'" alt="" border="0"><'+'/div>'+"\n";
			if (action[25] != "") thisHTML += '<'+'div class="textpad" align="center"><'+'a href="#" onclick="self.close();">Close Window<'+'/a><'+'/div>'+"\n";
			thisHTML += "<"+"/body>\n\n<"+"/html>";
			MPStoreWinStngs2.html = thisHTML;
			MPStoreWinStngs2.type = "image";
			thisURL = "";
			}
		MPStoreWinStngs2.name = winName;
		MPStoreWinStngs2.options = windowOptions;
		MPStoreWinStngs2.url = thisURL;
		MPStoreWinStngs2.redirect = redirect;
		var thisPopup = window.open(thisURL, winName, windowOptions);
		MPStoreOpenWin2[MPStoreWinStngs2.name] = thisPopup;
		setTimeout("MPTriggerDelayWin2()", 400);
		}
	}
function MPTriggerDelayWin2() {
	if (typeof MPStoreOpenWin2[MPStoreWinStngs2.name] == "object") {
		var thisPopup = MPStoreOpenWin2[MPStoreWinStngs2.name];
		if (thisPopup.closed) thisPopup = window.open(MPStoreWinStngs2.url, MPStoreWinStngs2.name, MPStoreWinStngs2.options);
		if (thisPopup && typeof thisPopup == "object") {
			if (MPStoreWinStngs2.type == "image" && MPStoreWinStngs2.html != "") {
				if (!thisPopup.closed) {
					thisPopup.document.open();
					thisPopup.document.write(MPStoreWinStngs2.html);
					thisPopup.document.close();
					}
				}
			}
		if (MPStoreWinStngs2.redirect != "") window.location = MPStoreWinStngs2.redirect;
		MPStoreOpenWin2[MPStoreWinStngs2.name] = thisPopup;
		if (thisPopup && typeof thisPopup == "object") if (!thisPopup.closed && MPStoreWinStngs2.focus) thisPopup.focus();
		MPStoreWinStngs2.url = "";
		MPStoreWinStngs2.name = "";
		MPStoreWinStngs2.options = "";
		MPStoreWinStngs2.focus = true;
		MPStoreWinStngs2.html = "";
		MPStoreWinStngs2.redirect = "";
		}
	}
//-->
// OpenPopUpLite 2.0.1 action by Nate Baldwin, www.mindpalette.com, copyright 2004
if (typeof MPStoreOpenWin2 == "undefined") MPStoreOpenWin2 = new Array();
function MPOpenPopupLite(action) {
	var posX = 0;
	var posY = 0;
	if (action[4] == true) {
		posX = Math.round((screen.availWidth/2)-(action[2]/2));
		posY = Math.round((screen.availHeight/2)-(action[3]/2));
		} else {
		posX = action[12];
		posY = action[13];
		}
	if (action[16] == true) {
		posX = 0;
		posY = 0;
		action[2] = screen.availWidth;
		action[3] = screen.availHeight;
		}
	for (i=5; i<12; i++) {
		action[i] == true ? action[i] = "yes" : action[i] = "no";
		}
	var windowOptions = "";
	windowOptions += "width=" + action[2];
	windowOptions += ",height=" + action[3];
	windowOptions += ",resizable=" + action[11];
	windowOptions += ",scrollbars=" + action[5];
	windowOptions += ",menubar=" + action[6];
	windowOptions += ",toolbar=" + action[9];
	windowOptions += ",directories=" + action[7];
	windowOptions += ",location=" + action[10];
	windowOptions += ",status=" + action[8];
	windowOptions += ",left=" + posX;
	windowOptions += ",top=" + posY;
	var thisPopup = window.open(action[15], action[1], windowOptions);
	MPStoreOpenWin2[action[1]] = thisPopup;
	if (action[14] != "" && action[14] != "#" && action[14] != "(EmptyReference!)" && action[14] != "(Empty Reference!)")
		window.location = action[14];
	}
//-->
//PopOnce Action v1.0 AardActions by Jim Aardvark. Copyright 2003. seanclayton@tinkers-inc.co.uk
JAStoreWindowValues = new Array();
function JAPopOnce(action){
	if (action[12] && action[12] != "" && action[12] != 0) {
		action[12] = parseInt(action[12]);
    	var expDate = new Date();
   		var monthCount = expDate.getTime() + (1000 * 60 * 60 * 24 * 31 * action[12]);
    	expDate.setTime(monthCount);
    	expDate = expDate.toGMTString();
    	} else expDate = false;
    
    var xpos = 0;
    var ypos = 0;
    
    if (action[13] == true) {
	posX = ((screen.width/2)-(action[3]/2));
	posY = ((screen.height/2)-(action[4]/2));
	} else {
	posX = action[14];
	posY = action[15];
	}
	
    var wf = "";	
	wf = wf + "width=" + action[3];
	wf = wf + ",height=" + action[4];
	wf = wf + ",resizable=" + (action[5] ? "yes" : "no");
	wf = wf + ",scrollbars=" + (action[6] ? "yes" : "no");
	wf = wf + ",menubar=" + (action[7] ? "yes" : "no");
	wf = wf + ",toolbar=" + (action[8] ? "yes" : "no");
	wf = wf + ",directories=" + (action[9] ? "yes" : "no");
	wf = wf + ",location=" + (action[10] ? "yes" : "no");
	wf = wf + ",status=" + (action[11] ? "yes" : "no");
	wf = wf + ",left=" + posX;
	wf = wf + ",top=" + posY;
	
	if (document.cookie.indexOf("PopOnce") == -1){
		JAStoreWindowValues['link'] = action[1];
		JAStoreWindowValues['target'] = action[2];
		JAStoreWindowValues['settings'] = wf;
		var delay = action[16] * 1000;
		var thisCookie = "PopOnce=general;";
		if (expDate) thisCookie += " expires=" + expDate + ";";
		document.cookie = thisCookie;
		setTimeout("JALaunchWindow()", delay);
	}
}
function JALaunchWindow(){
	window.open(JAStoreWindowValues['link'], JAStoreWindowValues['target'], JAStoreWindowValues['settings']);
}
// RadioToMenu Action v1.1 by Nate Baldwin, www.mindpalette.comm, copyright 2005...
function MPRadioToMenu(action) {
var thisForm = action[1];
var controlBtn = eval("document."+thisForm+"."+action[2]);
var compValue = action[3];
var viewMenu = eval("document."+thisForm+"."+action[4]);
var selectedOne = -1;
for (x=0; x<controlBtn.length; x++) {
	if (controlBtn[x].checked && controlBtn[x].value == compValue) {
		selectedOne = x;
		viewMenu.options.length = 0;
		var labelArray = new Array();
		var valueArray = new Array();
		var n = 0;
		for (i=5; i<44; i+=2) {
			if (action[i] != "" && action[i] != "#" && action[i] != "EmptyReference!" && action[i] != "Empty Reference!" && action[i] != null) {
				labelArray[n] = action[i];
				if (action[i+1] != "") {
					valueArray[n] = action[i+1];
					} else {
					valueArray[n] = action[i];
					}
				n++;
				}
			}
		for(i=0; i<labelArray.length; i++) {
			viewMenu.options[i] = new Option(labelArray[i], valueArray[i]);
			}
		viewMenu.options[0].selected = true;
		}
	}
if (selectedOne > -1) setTimeout("MPUpdateRadioBtn2('"+action[1]+"', '"+action[2]+"', '"+selectedOne+"')", 100);
}
function MPUpdateRadioBtn2(thisForm, thisGroup, thisSelected) {
	var thisRadio = eval("document."+thisForm+"."+thisGroup);
	for (i=0; i<thisRadio.length; i++) {
		thisRadio[i].checked = false;
		}
	thisRadio[thisSelected].checked = true;
	}
//-->
function CSWinResLocation(action){
if (window.screen){ 
	var width=screen.width 
	var er="Empty Reference"
		if (width<=640) { 
			if(action[4]!=""&&action[4].indexOf(er)==-1) self.location.replace(action[4])
		} 
		else if (width<=800) { 
			if(action[3]!=""&&action[3].indexOf(er)==-1) self.location.replace(action[3])
		} 
		else if (width<=1024) { 
			if(action[2]!=""&&action[2].indexOf(er)==-1)	self.location.replace(action[2])
	
		} 
		else if (width<=1280){ 
			if(action[1]!=""&&action[1].indexOf(er)==-1) self.location.replace(action[1])
		} 
		else if(width>1280){
			if(action[5]!=""&&action[5].indexOf(er)==-1) self.location.replace(action[5])
		}
	}
}
// ReadImageAndLink BETA by Nate Baldwin, www.mindpalette.com, copyright 2003-08-10
var MPSetImageAndLinkArray1 = new Array();
function MPImgLinkGet(action) {
	var thisImage = action[1];
	if (thisImage != "" && thisImage != "(Empty Reference!)" && thisImage != "(EmptyReference!)" && thisImage != "#") {
		if (typeof MPSetImageAndLinkArray1 != "undefined") {
			if (typeof MPSetImageAndLinkArray1[thisImage] != "undefined") {
				var imageURL = MPSetImageAndLinkArray1[thisImage][0];
				var linkURL = MPSetImageAndLinkArray1[thisImage][1];
				var linkTarget = MPSetImageAndLinkArray1[thisImage][2];
				if (linkURL != "" && linkURL != "(Empty Reference!)" && linkURL != "(EmptyReference!)" && linkURL != "#") {
					if (linkTarget != "") {
						var frameFound = false;
						for (i=0; i<parent.frames.length; i++) {
							if (parent.frames[i].name == linkTarget) {
								frameFound = true;
								break;
								}
							}
						if (frameFound == true)
							parent.frames[linkTarget].location = linkURL;
							else window.open(linkURL, linkTarget, "");
						} else window.location = linkURL;
					}
				}
			}
		}
	}
//-->
// SetImageAndLink BETA by Nate Baldwin, www.mindpalette.com, copyright 2003-08-10
var MPSetImageAndLinkArray1 = new Array();
function MPImgLinkSet(action) {
	var imageName = action[1];
	var imageURL = action[2];
	var linkURL = action[3];
	var linkTarget = action[4];
	if (imageName != "" && imageName != "(Empty Reference!)" && imageName != "(EmptyReference!)" && imageName != "#") {
		MPSetImageAndLinkArray1[imageName] = new Array(imageURL, linkURL, linkTarget);
		if (imageURL != "" && imageURL != "(Empty Reference!)" && imageURL != "(EmptyReference!)" && imageURL != "#") {
			document.images[imageName].src = imageURL;
			}
		}
	}
//-->
// SlideShowKit 1.5.1 by Nate Baldwin, www.mindpalette.com, April 27, 2004
// Slide Show Detail List...
function MPImgDetailList2(action) {
	var thisGroup = action[1];
	if (thisGroup == "") thisGroup = "default";
	if (MP_SSKParam1[thisGroup] != "undefined") {
		if (MPValidURLString(action[2])) {
			var thisSlide = new Object();
			thisSlide.imgurl = action[2];
			thisSlide.linkurl = (MPValidURLString(action[3])) ? action[3] : "";
			thisSlide.caption = action[4];
			thisSlide.descript = action[5];
			MP_SSKParam1[thisGroup].imagelist[MP_SSKParam1[thisGroup].imagelist.length] = thisSlide;
			}
		}
	}
//-->
// Slideshow Kit 1.5.1 by Nate Baldwin, www.mindpalette.com, July 14, 2004
// Slide Show DIV Action...
function MPSSKWriteDiv2(action) {
	var id = action[1];
	var content = action[2];
	var html = "";
	if (id != "") {
		html = '<'+'div id="'+id+'">'+content+'<'+'/div>'
		}
	if (html != "") document.write(html);
	}
//-->
// Slideshow Kit 1.5.1 by Nate Baldwin, www.mindpalette.com, April 27, 2004
// Slide Show Folder Action...
function MPImgLinkFolder2(action) {
	var thisGroup = (action[1] != "") ? action[1] : "default";
	if (typeof MP_SSKParam1[thisGroup] != "undefined") {
		var imgDir = (MPValidURLString(action[2])) ? action[2] : "";
		imgDir = MPCheckDirFormat(imgDir);
		var startNum = (action[3] != "") ? parseInt(action[3]) : 1;
		var endNum = (action[4] != "") ? parseInt(action[4]) : 0;
		var imgExt = (action[5] != "") ? MPCheckExtFormat(action[5]) : "jpg";
		var linkImgs = action[6];
		var linkExt = (action[7] != "") ? MPCheckExtFormat(action[7]) : "html";
		var linkDir = (MPValidURLString(action[8])) ? action[8] : "";
		linkDir = MPCheckDirFormat(linkDir);
		if (endNum > 0 && endNum > startNum && imgDir != "") {
			for (i=startNum; i<=endNum; i++) {
				var thisSlide = new Object();
				thisSlide.imgurl = imgDir+i+"."+imgExt;
				thisSlide.linkurl = (linkImgs == true && linkDir != "") ? linkDir+i+"."+linkExt : "";
				thisSlide.caption = "";
				thisSlide.descript = "";
				MP_SSKParam1[thisGroup].imagelist[MP_SSKParam1[thisGroup].imagelist.length] = thisSlide;
				}
			}
		}
	}
//-->
// Slideshow Kit 1.5.2 by Nate Baldwin, www.mindpalette.com, June 15, 2005
// With modification to allow image name prefix string
// Slide Show Folder Prefix Action...
function MPImgLinkFolderPrefix(action) {
	var thisGroup = (action[1] != "") ? action[1] : "default";
	if (typeof MP_SSKParam1[thisGroup] != "undefined") {
		var imgDir = (MPValidURLString(action[2])) ? action[2] : "";
		imgDir = MPCheckDirFormat(imgDir);
		var startNum = (action[3] != "") ? parseInt(action[3]) : 1;
		var endNum = (action[4] != "") ? parseInt(action[4]) : 0;
		var imgExt = (action[5] != "") ? MPCheckExtFormat(action[5]) : "jpg";
		var linkImgs = action[6];
		var linkExt = (action[7] != "") ? MPCheckExtFormat(action[7]) : "html";
		var linkDir = (MPValidURLString(action[8])) ? action[8] : "";
		var namePrefix = action[9];
		var pad = (parseInt(action[10]) != 'NaN') ? parseInt(action[10]) : 0;
		linkDir = MPCheckDirFormat(linkDir);
		if (endNum > 0 && endNum > startNum && imgDir != "") {
			for (i=startNum; i<=endNum; i++) {
				var thisSlide = new Object();
				var numString = i+'';
				var remainder = pad - (numString.length + namePrefix.length);
				var padString = '';
				for (var x=0; x<remainder; x++) {
					padString = padString + '0';
					}
				var thisFileName = namePrefix+padString+numString+'.'+imgExt;
				thisSlide.imgurl = imgDir+thisFileName;
				thisSlide.linkurl = (linkImgs == true && linkDir != "") ? linkDir+i+"."+linkExt : "";
				thisSlide.caption = "";
				thisSlide.descript = "";
				MP_SSKParam1[thisGroup].imagelist[MP_SSKParam1[thisGroup].imagelist.length] = thisSlide;
				}
			}
		}
	}
//-->
// SlideShowKit 1.5.2 by Nate Baldwin, mindpalette.com - May 19, 2005
// Slide Show Jump Action...
function MPJump2SlideX(action) {
	var group = (action[1] != "") ? action[1] : "default";
	var imgurl = action[2];
	if (typeof MP_SSKParam1[group] != "undefined") {
		if (MPValidURLString(imgurl)) {
			MP_SSKParam1[group].MPSetToggle("pause");
			MP_SSKParam1[group].autoscroll.run = false;
			var imageList = MP_SSKParam1[group].imagelist;
			var nextIndex = -1;
			var stopScript = false;
			var currentImage = MPGetFileName(imgurl);
			for (n=0; n<imageList.length; n++) {
				if (imageList[n] != "") {
					var testImage = MPGetFileName(imageList[n].imgurl);
					if (testImage == currentImage) {
						nextIndex = n;
						break;
						}
					}
				}
			if (nextIndex > -1) {
				var options = MP_SSKParam1[group].imageopts;
				var img = options.imgname;
				if (options.nextimg != "" && options.nextaltsrc != "" && options.nextimgsrc != "") {
					var fixImage = MP_SSKParam1[group].imageopts.nextimg;
					if (nextIndex == (imageList.length-1))
						document.images[fixImage].src = options.nextaltsrc;
						else document.images[fixImage].src = options.nextimgsrc;
					}
				if (options.previmg != "" && options.prevaltsrc != "" && options.previmgsrc != "") {
					var fixImage = MP_SSKParam1[group].imageopts.previmg;
					if (nextIndex == 0)
						document.images[fixImage].src = options.prevaltsrc;
						else document.images[fixImage].src = options.previmgsrc;
					}
				if (stopScript == false) {
					document.images[img].src = imageList[nextIndex].imgurl;
					MP_SSKParam1[group].MPUpdateImgMenu(nextIndex);
					MP_SSKParam1[group].MPUpdateCaptDescDivs(nextIndex);
					}
				}
			}
		}
	}
//-->
// SlideShowKit 1.5.1 by Nate Baldwin, www.mindpalette.com - April 27, 2004
// Slide Show Link Action...
function MPImgLinkTrigger1(action) {
	var imgGroup = action[1];
	var linkType = "";
	if (action[2] == "1") linkType = "popup";
		else if (action[2] == "2") linkType = "aiw";
		else linkType = "link";
	var urlFound = "";
	var aiwImg = "";
	if (typeof MP_SSKParam1[imgGroup] != "undefined") {
		var currentImg = MPGetFileName(document.images[MP_SSKParam1[imgGroup].imageopts.imgname].src);
		for (n=0; n<MP_SSKParam1[imgGroup].imagelist.length; n++) {
			var testImage = MPGetFileName(MP_SSKParam1[imgGroup].imagelist[n].imgurl);
			if (testImage == currentImg) {
				if (linkType == "aiw") {
					if (action[20] != "") {
						aiwImg = MPCheckDirFormat(action[20])+testImage;
						break;
						}
					} else if (testImage == currentImg && MP_SSKParam1[imgGroup].imagelist[n].linkurl != "") {
					urlFound = MP_SSKParam1[imgGroup].imagelist[n].linkurl;
					break;
					}
				}
				
			}
		}
	if (urlFound != "" || aiwImg != "") {
		if (action[25] == true) {
			MP_SSKParam1[imgGroup].MPSetToggle("pause");
			MP_SSKParam1[imgGroup].autoscroll.run = false;
			if (typeof MP_SSKParam1[imgGroup].autoscroll.timer == "number") {
				clearTimeout(MP_SSKParam1[imgGroup].autoscroll.timer);
				MP_SSKParam1[imgGroup].autoscroll.timer = false;
				}
			}
		if (linkType == "link") {
			var frameFound = false;
			var linkTarget = action[19];
			if (linkTarget != "") {
				for (i=0; i<parent.frames.length; i++) {
					if (parent.frames[i].name == linkTarget) {
						frameFound = true;
						break;
						}
					}
				}
			if (frameFound == true)
				parent.frames[linkTarget].location = urlFound;
				else window.location = urlFound;
			} else {
			var posX = action[14];
			var posY = action[15];
			var winWidth = action[4];
			var winHeight = action[5];
			var setWidth = true;
			var setHeight = true;
			if (winWidth == "" || winWidth == 0 || winWidth == "0") setWidth = false;
			if (winHeight == "" || winHeight == 0 || winHeight == "0") setHeight = false;
			if (action[6] == true) {
				if (setWidth == true) posX = Math.round((screen.availWidth/2)-(winWidth/2));
				if (setHeight == true) posY = Math.round((screen.availHeight/2)-(winHeight/2));
				}
			if (action[17] == true || action[16] == true) {
				posX = 0;
				winWidth = screen.availWidth;
				}
			if (action[18] == true || action[16] == true) {
				posY = 0;
				winHeight = screen.availHeight;
				}
			if (winWidth > (screen.availWidth - posX)) winWidth = (screen.availWidth - posX);
			if (winHeight > (screen.availHeight - posY)) winHeight = (screen.availHeight - posY);
			for (i=7; i<14; i++) {
				action[i] == true ? action[i] = "yes" : action[i] = "no";
				}
			var windowOptions = "";
			if (setWidth == true) windowOptions += "width=" + winWidth;
			if (setHeight == true) {
				if (setWidth == true) windowOptions += ",";
				windowOptions += "height=" + winHeight;
				}
			if (setWidth == true || setHeight == true) windowOptions += ",";
			windowOptions += "resizable=" + action[13];
			windowOptions += ",scrollbars=" + action[7];
			windowOptions += ",menubar=" + action[8];
			windowOptions += ",toolbar=" + action[11];
			windowOptions += ",directories=" + action[9];
			windowOptions += ",location=" + action[12];
			windowOptions += ",status=" + action[10];
			windowOptions += ",left=" + posX;
			windowOptions += ",top=" + posY;
			if (linkType != "aiw") {
				var thisPopup = window.open(urlFound, action[3], windowOptions);
				if (thisPopup && typeof thisPopup == "object") thisPopup.focus();
				} else {
				var code = "";
				code += '<'+'!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"'+'>'+"\n\n";
				code += '<'+'html'+'>'+"\n\n";
				code += '<'+'head'+'>'+"\n";
				code += '<'+'meta http-equiv="content-type" content="text/html;charset=iso-8859-1"'+'>'+"\n";
				code += '<'+'title'+'>'+action[21]+'<'+'/title'+'>'+"\n";
				if (action[23] == true && action[24] != "") {
					code += '<'+'style type="text/css" media="screen"'+'><'+'!--'+"\n";
					code += 'a { color: '+action[24]+'; font-size: 12px !important; font-family: Verdana, Arial, Helvetica }'+"\n";
					code += '--'+'><'+'/style'+'>'+"\n";
					}
				code += '<'+'/head'+'>'+"\n\n";
				code += '<'+'body bgcolor="'+action[22]+'" leftmargin="0" marginheight="0" marginwidth="0" topmargin="0"'+'>'+"\n";
				code += '<'+'table width="100%" border="0" cellspacing="0" cellpadding="0" height="100%"'+'>'+"\n";
				code += '<'+'tr'+'>'+"\n";
				code += '<'+'td align="center" valign="middle"'+'><'+'img src="'+aiwImg+'" alt="" border="0"'+'>'+"\n";
				if (action[23] == true && action[24] != "") code += '<br'+'><'+'a href="javascript:self.close();"'+'>'+'close window'+'<'+'/a'+'>'+"\n";
				code += '<'+'/td'+'><'+'/tr'+'>'+"\n";
				code += '<'+'/table'+'>'+"\n";
				code += '<'+'/body'+'>'+"\n\n";
				code += '<'+'/html'+'>';
				var thisPopup = window.open("", action[3], windowOptions);
				if (thisPopup && typeof thisPopup == "object") {
					thisPopup.document.open();
					thisPopup.document.write(code);
					thisPopup.document.close();
					thisPopup.focus();
					}
				}
			}
		}
	}
//-->
// SlideShowKit 1.5.1 by Nate Baldwin, www.mindpalette.com, April 27, 2004
// Slide Show Link List Action...
function MPImgLinkCapt2(action) {
	var thisGroup = action[1];
	if (thisGroup == "") thisGroup = "default";
	if (MP_SSKParam1[thisGroup] != "undefined") {
		for (i=2; i<=16; i+=3) {
			if (MPValidURLString(action[i])) {
				var thisSlide = new Object();
				thisSlide.imgurl = action[i];
				thisSlide.linkurl = (MPValidURLString(action[i+1])) ? action[i+1] : "";
				thisSlide.caption = action[i+2];
				thisSlide.descript = "";
				MP_SSKParam1[thisGroup].imagelist[MP_SSKParam1[thisGroup].imagelist.length] = thisSlide;
				}
			}
		}
	}
//-->
// SlideShowKit 1.5.1 by Nate Baldwin, www.mindpalette.com, April 27, 2004
// Slide Show Link List Action...
function MPImgLinkList2(action) {
	var thisGroup = action[1];
	if (thisGroup == "") thisGroup = "default";
	if (MP_SSKParam1[thisGroup] != "undefined") {
		for (i=2; i<=21; i+=2) {
			if (MPValidURLString(action[i])) {
				var thisSlide = new Object();
				thisSlide.imgurl = action[i];
				thisSlide.linkurl = (MPValidURLString(action[i+1])) ? action[i+1] : "";
				thisSlide.caption = "";
				thisSlide.descript = "";
				MP_SSKParam1[thisGroup].imagelist[MP_SSKParam1[thisGroup].imagelist.length] = thisSlide;
				}
			}
		}
	}
//-->
// SlideShowKit 1.5.1 by Nate Baldwin, www.mindpalette.com, April 27, 2004
// Slide Show List Action...
function MPLoadImgLst2(action) {
	var thisGroup = action[1];
	if (thisGroup == "") thisGroup = "default";
	if (MP_SSKParam1[thisGroup] != "undefined") {
		for (i=2; i<=21; i++) {
			if (MPValidURLString(action[i])) {
				var thisSlide = new Object();
				thisSlide.imgurl = action[i];
				thisSlide.linkurl = "";
				thisSlide.caption = "";
				thisSlide.descript = "";
				MP_SSKParam1[thisGroup].imagelist[MP_SSKParam1[thisGroup].imagelist.length] = thisSlide;
				}
			}
		}
	}
//-->
// Slideshow Kit 1.5.1 by Nate Baldwin, www.mindpalette.com, April 27, 2004
// Slide Show Menu Action...
function MPImgSeqMenu1(action) {
	var imgGroup = action[1];
	var visRows = (parseInt(action[2]) > 0) ? action[2] : "1";
	if (typeof MP_SSKParam1[imgGroup] != "undefined") {
		var currentImgName = MP_SSKParam1[imgGroup].imageopts.imgname;
		if (typeof document.images[currentImgName] != "undefined")
			var currentImg = document.images[currentImgName].src;
			else currentImg = "";
		var formHTML = "";
		currentImg = MPGetFileName(currentImg);
		var currentFound = false;
		var thisImgList = MP_SSKParam1[imgGroup].imagelist;
		var thisTotal = thisImgList.length;
		if (thisImgList.length > 0) {
			formHTML += "\n"+'<'+'form name="'+imgGroup+'_form"'+">\n";
			formHTML += '<'+'select name="'+imgGroup+'_menu" size="'+visRows+'" ';
			formHTML += 'onchange="MP_SSKParam1[\''+imgGroup+'\'].MPJump2ImgNum2(this, \''+imgGroup+'\');">'+"\n";
			for (n=0; n<thisImgList.length; n++) {
				var thisImg = MPGetFileName(thisImgList[n].imgurl);
				if (thisImg != "") {
					formHTML += '<'+'option ';
					if (currentImg == thisImg && currentFound == false) {
						formHTML += "selected ";
						currentFound = true;
						}
					var menuText = "";
					if (action[3] == "1") menuText = (n+1)+' of '+thisTotal;
						else if (action[3] == "2") {
						var caption = (thisImgList[n].caption != "") ? thisImgList[n].caption : thisImg;
						menuText = (n+1)+". "+caption;
						} else menuText = (n+1)+". "+thisImg;
					formHTML += 'value="'+n+'">'+menuText+'<'+'/option'+">\n";
					}
				}
			formHTML += '<'+'/select'+'>';
			formHTML += '<'+'/form'+'>';
			}
		document.write(formHTML);
		}
	}
//-->
// Slideshow Kit 1.5.2 by Nate Baldwin, www.mindpalette.com, August 31, 2004
// Slide Show Options Action...
function MPGetFileName(startString) {
	thisString = startString+"";
	if (thisString.lastIndexOf('/') != -1) {
		var nameStart = thisString.lastIndexOf('/') + 1;
		var nameEnd = thisString.length;
		var thisFile = thisString.substring(nameStart, nameEnd);
		} else var thisFile = thisString;
	return thisFile;
	}
function MPValidURLString(testString) {
	var validString = true;
	if (testString == "") validString = false;
		else if (testString == "#") validString = false;
		else if (testString == "(EmptyReference!)") validString = false;
		else if (testString == "(Empty Reference!)") validString = false;
	if (validString == true) return true;
		else return false;
	}
function MPCheckExtFormat(thisString) {
	if (thisString.charAt(0) == ".") thisString = thisString.substring(1);
	return thisString;
	}
function MPCheckDirFormat(thisString) {
	if (thisString != "") {
		if (thisString.charAt(thisString.length - 1) == "/") thisString = thisString.substring(0, thisString.length - 1);
		var dirArray = thisString.split("/");
		var lastIndex = dirArray.length - 1;
		thisString = "";
		for (n=0; n<dirArray.length; n++) {
			var addThisBlock = true;
			if (n == lastIndex) {
				var testArray = dirArray[lastIndex].split(".");
				if (testArray.length > 1) addThisBlock = false;
				}
			if (addThisBlock == true) thisString += dirArray[n]+"/";
			}
		if (thisString.charAt(thisString.length - 1) != "/") thisString += "/";
		}
	return thisString;
	}
function MPJump2ImgNum2(menu, group) {
	var imgNum = menu.options[menu.selectedIndex].value;
	if (imgNum != "") {
		var imgSrc = this.imagelist[imgNum].imgurl;
		document.images[this.imageopts.imgname].src = imgSrc;
		if (this.imageopts.nextaltsrc != "" && this.imageopts.nextimgsrc != "" && this.imageopts.nextimg != "") {
			if (imgNum == (this.imagelist.length - 1))
				document.images[this.imageopts.nextimg].src = this.imageopts.nextaltsrc;
				else document.images[this.imageopts.nextimg].src = this.imageopts.nextimgsrc;
			}
		if (this.imageopts.prevaltsrc != "" && this.imageopts.previmgsrc != "" && this.imageopts.previmg != "") {
			if (imgNum == 0)
				document.images[this.imageopts.previmg].src = this.imageopts.prevaltsrc;
				else document.images[this.imageopts.previmg].src = this.imageopts.previmgsrc;
			}
		this.MPUpdateCaptDescDivs(imgNum);
		this.MPSetToggle("pause");
		this.autoscroll.run = false;
		if (typeof this.autoscroll.timer == "number") {
			clearTimeout(this.autoscroll.timer);
			this.autoscroll.timer = false;
			}
		}
	}
function MPUpdateImgMenu(newIndex) {
	var thisForm = this.imagegroup+"_form";
	var thisMenu = this.imagegroup+"_menu";
	if (typeof document.forms[thisForm] != "undefined") {
		if (typeof document.forms[thisForm].elements[thisMenu].options[newIndex] != "undefined") {
			for (m=0; m<document.forms[thisForm].elements[thisMenu].length; m++) {
				if (m == (newIndex)) document.forms[thisForm].elements[thisMenu].options[m].selected = true;
					else document.forms[thisForm].elements[thisMenu].options[m].selected = false;
				}
			}
		}
	}
function MPSetToggle(type) {
	if (this.imageopts.playbtn != "" && this.imageopts.playsrc != "" && this.imageopts.pausesrc != "") {
		if (type == "play") document.images[this.imageopts.playbtn].src = this.imageopts.pausesrc;
			else document.images[this.imageopts.playbtn].src = this.imageopts.playsrc;
		}
	}
function MPAutoScrollImgSeq1(group) {
	if (typeof MP_SSKParam1[group] != "undefined") {
		if (MP_SSKParam1[group].autoscroll.run == true) {
			var thisIndex = MP_SSKParam1[group].autoscroll.index;
			if (MP_SSKParam1[group].imageopts.stop == true) {
				if (MP_SSKParam1[group].autoscroll.reverse == true) {
					if (thisIndex < 1) {
						MP_SSKParam1[group].autoscroll.run = false;
						if (MP_SSKParam1[group].imageopts.alert == true) alert(MP_SSKParam1[group].imageopts.startalert);
						}
					} else {
					if (thisIndex >= (MP_SSKParam1[group].imagelist.length - 1)) {
						MP_SSKParam1[group].autoscroll.run = false;
						if (MP_SSKParam1[group].imageopts.alert == true) alert(MP_SSKParam1[group].imageopts.endalert);
						}
					}
				}
			}
		if (MP_SSKParam1[group].autoscroll.run == true) {
			var nextIndex = 0;
			if (MP_SSKParam1[group].autoscroll.stall == true)
				nextIndex = thisIndex;
				else {
				if (MP_SSKParam1[group].autoscroll.reverse == false) {
					if (thisIndex < (MP_SSKParam1[group].imagelist.length - 1))
						nextIndex = (MP_SSKParam1[group].autoscroll.index + 1);
					} else {
					if (MP_SSKParam1[group].autoscroll.index > 0) nextIndex = thisIndex - 1;
						else nextIndex = (MP_SSKParam1[group].imagelist.length - 1);
					}
				}
			if (MP_SSKParam1[group].imagelist[nextIndex][0] != "")
				document.images[MP_SSKParam1[group].imageopts.imgname].src = MP_SSKParam1[group].imagelist[nextIndex].imgurl;
			var timer = setTimeout("this.MPAutoScrollImgSeq1('"+group+"')", MP_SSKParam1[group].autoscroll.delay);
			MP_SSKParam1[group].autoscroll.index = nextIndex;
			MP_SSKParam1[group].autoscroll.timer = timer;
			if (MP_SSKParam1[group].autoscroll.stall == false) {
				MP_SSKParam1[group].MPUpdateImgMenu(nextIndex);
				MP_SSKParam1[group].MPUpdateCaptDescDivs(nextIndex);
				}
			MP_SSKParam1[group].autoscroll.stall = false;
			} else {
			if (typeof MP_SSKParam1[group].autoscroll.timer == "number") {
				clearTimeout(MP_SSKParam1[group].autoscroll.timer);
				MP_SSKParam1[group].autoscroll.timer = false;
				}
			MP_SSKParam1[group].MPSetToggle("pause");
			MP_SSKParam1[group].autoscroll.run = false;
			}
		}
	}
function MPDivTextWrite2(div, text) {
	if (document.layers) {
		if (typeof eval("document."+div+".document") != "undefined") {
			with (eval("document."+div+".document")) {
				open();
				write("<"+"HTML"+"><"+"HEAD"+"><"+"/HEAD"+"><"+"BODY"+">"+text+"<"+"/BODY"+"><"+"/HTML"+">");
				close();
				}
			}
		} else if (document.all && typeof document.all[div] != "undefined") {
		document.all[div].innerHTML = text;
		} else if (document.getElementById && !document.all && typeof document.getElementById(div) != "undefined") {
		document.getElementById(div).innerHTML = text;
		}
	}
function MPParseCountDivText(text, count, total) {
	var regexp = /\[count\]/g
	text = text.replace(regexp, count);
	var regexp = /\[total\]/g
	text = text.replace(regexp, total);
	return text;
	}
function MPUpdateCaptDescDivs(nextIndex) {
	var thisCaptDiv = this.imageopts.captdiv;
	if (thisCaptDiv == "#") thisCaptDiv = "";
	var thisCaptTxt = this.imagelist[nextIndex].caption;
	if (thisCaptDiv != "" && thisCaptTxt != "") MPDivTextWrite2(thisCaptDiv, thisCaptTxt);
	var thisDescDiv = this.imageopts.descdiv;
	var thisDescTxt = this.imagelist[nextIndex].descript;
	if (thisDescDiv != "" && thisDescTxt != "") MPDivTextWrite2(thisDescDiv, thisDescTxt);
	var thisCountDiv = this.imageopts.countdiv;
	if (thisCountDiv == "#") thisCountDiv = "";
	var countTxtTmp = (this.imageopts.counttxt != thisCountDiv) ? this.imageopts.counttxt : "";
	if (countTxtTmp == "") countTxtTmp = "[count] of [total]";
	var thisCountTxt = MPParseCountDivText(countTxtTmp, parseInt(nextIndex)+1, this.imagelist.length);
	if (thisCountDiv != "" && thisCountTxt != " of ") MPDivTextWrite2(thisCountDiv, thisCountTxt);
	}
if (typeof MP_SSKParam1 == "undefined") var MP_SSKParam1 = new Array();
if (typeof MPImageSeqPreldArray1 == "undefined") var MPImageSeqPreldArray1 = new Array();
function MPSequence2Options(action) {
	var group = (action[1] != "") ? action[1] : "default";
	var imageopts = new Object();
	imageopts.stop = action[2];
	imageopts.alert = action[3];
	imageopts.startalert = (action[4] != "") ? action[4] : "beginning of slideshow";
	imageopts.endalert = (action[5] != "") ? action[5] : "end of slideshow";
	imageopts.nextimg = action[6];
	imageopts.nextaltsrc = (MPValidURLString(action[7])) ? action[7] : "";
	imageopts.previmg = action[8];
	imageopts.prevaltsrc = (MPValidURLString(action[9])) ? action[9] : "";
	imageopts.imgname = action[10];
	imageopts.nextimgsrc = (MPValidURLString(action[11])) ? action[11] : "";
	imageopts.previmgsrc = (MPValidURLString(action[12])) ? action[12] : "";
	imageopts.playbtn = action[13];
	imageopts.playsrc = (MPValidURLString(action[14])) ? action[14] : "";
	imageopts.pausesrc = (MPValidURLString(action[15])) ? action[15] : "";
	imageopts.captdiv = action[16];
	imageopts.descdiv = action[17];
	imageopts.countdiv = action[18];
	imageopts.counttxt = action[19];
	var autoscroll = new Object();
	autoscroll.run = false;
	autoscroll.index = 0;
	autoscroll.delay = 3000;
	autoscroll.reverse = false;
	autoscroll.timer = false;
	autoscroll.stall = false;
	var slideshow = new Object();
	slideshow.imagegroup = group;
	slideshow.imagelist = new Array();
	slideshow.imageopts = imageopts;
	slideshow.autoscroll = autoscroll;
	slideshow.MPJump2ImgNum2 = MPJump2ImgNum2;
	slideshow.MPAutoScrollImgSeq1 = MPAutoScrollImgSeq1;
	slideshow.MPUpdateImgMenu = MPUpdateImgMenu;
	slideshow.MPSetToggle = MPSetToggle;
	slideshow.MPUpdateCaptDescDivs = MPUpdateCaptDescDivs;
	MP_SSKParam1[group] = slideshow;
	var preldArray = new Array();
	var MPImageSeqPreldCount1 = MPImageSeqPreldArray1.length;
	if (action[7] != "") preldArray[preldArray.length] = action[7];
	if (action[9] != "") preldArray[preldArray.length] = action[9];
	if (action[11] != "") preldArray[preldArray.length] = action[11];
	if (action[12] != "") preldArray[preldArray.length] = action[12];
	if (action[14] != "") preldArray[preldArray.length] = action[14];
	if (action[15] != "") preldArray[preldArray.length] = action[15];
	if (preldArray.length > 0) {
		MPImageSeqPreldArray1[MPImageSeqPreldCount1] = new Array();
		for (i=0; i<preldArray.length; i++) {
			if (preldArray[i] != "") {
				MPImageSeqPreldArray1[MPImageSeqPreldCount1][i] = new Image();
				MPImageSeqPreldArray1[MPImageSeqPreldCount1][i].src = preldArray[i];
				}
			}
		}
	}
//-->
// SlideShowKit 1.5.1 by Nate Baldwin, www.mindpalette.com, April 27, 2004
// Slide Show Preload Action...
if (typeof MPImageSeqPreldArray1 == "undefined") var MPImageSeqPreldArray1 = new Array();
function MPImgSeqPrld2(action) {
	var group = (action[1] != "") ? action[1] : "default";
	var limit = action[2];
	var max = (action[3] > 0) ? action[3] : 10;
	MPImageSeqPreldCount1 = MPImageSeqPreldArray1.length;
	if (limit == false || max > MP_SSKParam1[group].imagelist.length) max = MP_SSKParam1[group].imagelist.length;
	MPImageSeqPreldArray1[MPImageSeqPreldCount1] = new Array();
	for (i=0; i<max; i++) {
		if (MP_SSKParam1[group].imagelist[i].imgurl != "") {
			MPImageSeqPreldArray1[MPImageSeqPreldCount1][i] = new Image();
			MPImageSeqPreldArray1[MPImageSeqPreldCount1][i].src = MP_SSKParam1[group].imagelist[i].imgurl;
			}
		}
	}
//-->
// SlideShowKit 1.5 by Nate Baldwin, mindpalette.com - June 28, 2006
// Slide Show Trigger Action 1.5.6
function MPTriggerImageSeq(action) {
	var group = (action[1] != "") ? action[1] : "default";
	var type = (action[2] == 1 || action[2] == "1") ? "next" : "prev";
	var type = "";
	if (action[2] == "1") type = "prev";
		else if (action[2] == "2") type = "play";
		else if (action[2] == "3") type = "pause";
		else if (action[2] == "4") type = "toggle";
		else if (action[2] == "5") type = "start";
		else if (action[2] == "6") type = "end";
		else type = "next";
	var delay = (parseInt(action[3]) > 0) ? parseInt(action[3]) : 3000;
	var reverse = action[4];
	if (typeof MP_SSKParam1[group] != "undefined") {
		if (type == "toggle") {
			if (MP_SSKParam1[group].autoscroll.run == false) type = "play";
				else type = "pause";
			}
		MP_SSKParam1[group].MPSetToggle(type);
		MP_SSKParam1[group].autoscroll.run = false;
		if (typeof MP_SSKParam1[group].autoscroll.timer == "number") {
			clearTimeout(MP_SSKParam1[group].autoscroll.timer);
			MP_SSKParam1[group].autoscroll.timer = false;
			}
		if (type != "pause") {
			var nextIndex = (type == "end") ? (MP_SSKParam1[group].imagelist.length - 1) : 0;
			var thisIndex = -1;
			var imageList = MP_SSKParam1[group].imagelist;
			var img = MP_SSKParam1[group].imageopts.imgname;
			var stopScript = false;
			var currentImage = document.images[img].src;
			currentImage = MPGetFileName(currentImage);
			for (n=0; n<imageList.length; n++) {
				if (imageList[n] != "") {
					var testImage = MPGetFileName(imageList[n].imgurl);
					if (testImage == currentImage) {
						thisIndex = n;
						break;
						}
					}
				}
			if (type == "next" || (type == "play" && !reverse)) {
				if (thisIndex >= (imageList.length - 1))
					nextIndex = 0;
					else if (typeof imageList[thisIndex + 1] == "undefined")
					nextIndex = 0;
					else if (imageList[thisIndex + 1].imgurl == "")
					nextIndex = 0;
					else nextIndex = thisIndex + 1;
				if (nextIndex == 0) {
					if (MP_SSKParam1[group].imageopts.stop == true) {
						if (thisIndex == (imageList.length - 1)) stopScript = true;
						nextIndex = (thisIndex >= 0) ? thisIndex : 0;
						}
					if (MP_SSKParam1[group].imageopts.alert == true && thisIndex == (imageList.length - 1))
						alert(MP_SSKParam1[group].imageopts.endalert);
					}
				} else if (type == "prev" || (type == "next" && reverse)) {
				if ((thisIndex - 1) < 0)
					nextIndex = imageList.length - 1;
					else nextIndex = thisIndex - 1;
				if (nextIndex == (imageList.length - 1)) {
					if (MP_SSKParam1[group].imageopts.stop == true) {
						stopScript = true;
						nextIndex = (thisIndex >= 0) ? thisIndex : 0;
						}
					if (MP_SSKParam1[group].imageopts.alert == true) alert(MP_SSKParam1[group].imageopts.startalert);
					}
				}
			var options = MP_SSKParam1[group].imageopts;
			if (type == "play" && !stopScript) {
				MP_SSKParam1[group].autoscroll.run = true;
				var startIndex = (thisIndex > 0) ? thisIndex : 0;
				MP_SSKParam1[group].autoscroll.index = startIndex;
				MP_SSKParam1[group].autoscroll.delay = delay;
				MP_SSKParam1[group].autoscroll.reverse = reverse;
				MP_SSKParam1[group].autoscroll.stall = action[5];
				if (options.playbtn != "" && options.playsrc != "" && options.pausesrc != "")
					document.images[options.playbtn].src = options.pausesrc;
				MPAutoScrollImgSeq1(group);
				} else if (!stopScript) {
				if (options.nextimg != "" && options.nextaltsrc != "" && options.nextimgsrc != "") {
					var fixImage = MP_SSKParam1[group].imageopts.nextimg;
					if (nextIndex == (imageList.length-1))
						document.images[fixImage].src = options.nextaltsrc;
						else document.images[fixImage].src = options.nextimgsrc;
					}
				if (options.previmg != "" && options.prevaltsrc != "" && options.previmgsrc != "") {
					var fixImage = MP_SSKParam1[group].imageopts.previmg;
					if (nextIndex == 0)
						document.images[fixImage].src = options.prevaltsrc;
						else document.images[fixImage].src = options.previmgsrc;
					}
				if (stopScript == false) {
					document.images[img].src = imageList[nextIndex].imgurl;
					MP_SSKParam1[group].MPUpdateImgMenu(nextIndex);
					MP_SSKParam1[group].MPUpdateCaptDescDivs(nextIndex);
					}
				}
			}
		}
	}
//-->
function CSTempText(action){
	if(!action[7]){
		if(CStestDate(action[2],action[3])){
		document.write(action[1])
		}else{
		document.write(action[4])
		}
	}else{
		if(CStestDate(action[2],action[3])) {
		document.write('<img src='+action[6]+'>')
		}else if(action[5]!="#"){
		document.write('<img src='+action[5]+'>')
		}
		}
}
function CStestDate(action2,action3){
	stopInput=action2.split("/")
	startInput=action3.split("/")
	var stopDate= new Date(stopInput[2],stopInput[0]-1,stopInput[1])
	var startDate= new Date(startInput[2],startInput[0]-1,startInput[1])
	var currDate=new Date()
		if(currDate<stopDate&&currDate>startDate){
		return true
		}else{
		return false
		}
}
//TimeGreeting Action v1.0  AardActions by Jim Aardvark. Copyright 2003. seanclayton@tinkers-inc.co.uk
function JAtimeGreeting(action) 
{  
var day = new Date()
var hr = day.getHours()
if (hr < 12)
document.write(action[1])
if (hr >= 12)
if (hr < 18)
document.write(action[2])
if (hr >= 18)
document.write(action[3])
}
// A Michael Ahgren action - copyright 2002 WindowInnerSize 1.1
function CSWindowInnerSizeInit(action){
if (navigator.userAgent.indexOf("Opera")==-1){
var xCorr=0
var yCorr=0
if(navigator.userAgent.indexOf("Mozilla/4")!=-1&&navigator.userAgent.indexOf("Mac")!=-1){
xCorr=-15
yCorr=-30
}
if (action[3]){
maxSizeWindow(action)
}else{
	if (document.all ){
	top.window.resizeTo(screen.availWidth,screen.availHeight);
	var xChrome=screen.availWidth-document.body.clientWidth
	var yChrome=screen.availHeight-document.body.clientHeight
	changeX=action[4]-document.body.clientWidth
	changeY=action[5]-document.body.clientHeight
	top.window.resizeBy(changeX,changeY) 
}
else if ((document.layers||document.getElementById)&&!document.all){
	top.window.resizeTo(screen.availWidth,screen.availHeight);
	 top.window.innerHeight = action[5]+yCorr
 	top.window.innerWidth = action[4]+xCorr
}
if (action[6]){
	if (document.all){
		window.moveTo(parseInt(screen.availWidth/2-(action[4]+xChrome)/2),parseInt(screen.availHeight/2-(action[5]+yChrome)/2))
	}
	else if (document.layers||(document.getElementById&&!document.all)){
		window.moveTo(parseInt(screen.availWidth/2-window.outerWidth/2),parseInt(screen.availHeight/2-window.outerHeight/2))
}
}else{
	window.moveTo(action[1],action[2])
}
}
}
function maxSizeWindow(){
window.moveTo(0,0);
if (document.all){
top.window.resizeTo(screen.availWidth,screen.availHeight);
}
else if (document.layers||document.getElementById){
if (top.window.outerHeight<screen.availHeight||top.window.outerWidth<screen.availWidth)
{
 top.window.outerHeight = screen.availHeight;
 top.window.outerWidth = screen.availWidth;
}
}
}
}
function CSFieldValidate(action) { 
var form = action[1];
var elem = action[2];
var theEntry  = document.forms[form].elements[elem].value
var theFormElem = document.forms[form].elements[elem]
var badEntry = ""
	function theAlert () { 
	alert(action[6]);
	theFormElem.select();
	theFormElem.focus();
	}  	
	function isEmpty() { 
		if (theEntry == "") { 
		theAlert()
		} 	
	}
	function isNumber() { 
			if (theEntry == "") { 
			theAlert()
			} 		
		for (i=0; i<theEntry.length; i++) {  
			if (theEntry.charAt(i) < "0" || theEntry.charAt(i) > "9") {  
				badEntry = "notnumber"
				} 
			}  		
		if (badEntry == "notnumber") {
		theAlert()	
		}	
	} 
		
	function isAlpha() { 
			if (theEntry == "") { 
			theAlert()
			} 		
		for (i=0; i<theEntry.length; i++) {  
			if (theEntry.charAt(i) >= "0" && theEntry.charAt(i) <= "9") {  
				badEntry = "notalpha"
				} 
			}  		
		if (badEntry == "notalpha") {
		theAlert()	
		}	
	} 
				
	function requiredChars() {
	numofChars = theEntry.length
		if (numofChars != action[4]) {
			theAlert()
		} 
	}	
	function exactString() {
		if (theEntry != action[5]) {
			theAlert()
		} 
	}	
	
	function validEmail() {
		invalidChars = " /:,;"		
		if (theEntry == "") { 
			badEntry = "badEmail"
			}
		for (i=0; i < 5; i++)  {
			badChar = invalidChars.charAt(i)
				if (theEntry.indexOf(badChar,0) > -1) {
				badEntry = "badEmail"
				}
		}	
	atsignLoc = theEntry.indexOf("@",1)
		if (atsignLoc == -1) {
			badEntry = "badEmail"
		}		
		if (theEntry.indexOf("@",atsignLoc+1) > -1) {
		badEntry = "badEmail"
		}
	dotLoc = theEntry.indexOf(".",atsignLoc)
		if (dotLoc == -1) {
		badEntry = "badEmail"
		}
		if (dotLoc+3 > theEntry.length) {
		badEntry = "badEmail"
		}
		if (badEntry == "badEmail") {
		theAlert()
		}
	}
	function validCC() { 
	var theNumber = new Array(theEntry.length);
	var i = 0
	var total = 0
		for (i = 0; i < theEntry.length; ++i) {
		theNumber[i] = parseInt(theEntry.charAt(i))
		}
		for (i = theNumber.length -2; i >= 0; i-=2) {  
		theNumber[i] *= 2;							 
		if (theNumber[i] > 9) theNumber[i]-=9;			 
		}										 
		for (i = 0; i < theNumber.length; ++i) {
		total += theNumber[i];						 
		}	
		isinteger = total/10
		if(parseInt(isinteger)!=isinteger) {
		theAlert()
		}
	}
	
var type=action[3];
if(type==0) isEmpty()
else if(type==1) isNumber()
else if(type==2) isAlpha()
else if(type==3) requiredChars()
else if(type==4) exactString()
else if(type==5) validEmail()
else if(type==6) validCC()
}
		
function CSGetLayerPos(action) { 
	var layer = action[1];
	var x		= CSGetStylePos(layer, 0);
	var y		= CSGetStylePos(layer, 1);
	return new Array(x, y);
}
function CSGetFormElementValue(action) { 
	var form = action[1];
	var elem = action[2];
	return document.forms[form].elements[elem].value;
}
function initIArray() {
this.length = initIArray.arguments.length;
for (var i = 0; i < this.length; i++)
this[i+1] = initIArray.arguments[i]; 
}
function dailyImageURL(action) {
var dateArray = new
initIArray("Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday");
var today = new Date();
var day = dateArray[today.getDay()];
if (today.getDay() == 0) { day = "Sunday"; }
var img = null;
if (document.images) {
	if (!IsIE()&&CSBVers<5) img = CSFindElement(action[1],0);
	else img = document.images[action[1]];
		if (img) {
			if (day == "Monday" && action[2] != "(Empty Reference!)" && action[2] != "(EmptyReference!)") img.src = action[2]
			if (day == "Tuesday" && action[3] != "(Empty Reference!)" && action[3] != "(EmptyReference!)") img.src = action[3]
			if (day == "Wednesday" && action[4] != "(Empty Reference!)" && action[4] != "(EmptyReference!)") img.src = action[4]
			if (day == "Thursday" && action[5] != "(Empty Reference!)" && action[5] != "(EmptyReference!)") img.src = action[5]
			if (day == "Friday" && action[6] != "(Empty Reference!)" && action[6] != "(EmptyReference!)") img.src = action[6]
			if (day == "Saturday" && action[7] != "(Empty Reference!)" && action[7] != "(EmptyReference!)") img.src = action[7]
			if (day == "Sunday" && action[8] != "(Empty Reference!)" && action[8] != "(EmptyReference!)") img.src = action[8]
		}  
}  
}
CSImages=new Array();
function CSPreloadImage(action) {
	if (document.images) { CSImages[CSImages.length]=new Image(); CSImages[CSImages.length-1].src=action[1]; }
}
function CSRandomImg(action) { 
	var img = CSGetImage(action[1]);
	if (img) 
		{
		var v1 =  Math.floor(Math.random() * 10);
		var whichone = v1-(Math.floor(v1/3)*3);
		img.src = action[whichone+2];
		}
}
function CSSetImageURL(action) {
	var img=CSGetImage(action[1]);
	if (img) img.src=action[2];
}
function CSCloseWindow() { 
if (self.parent.frames.length != 0) {
	self.parent.close()	
	} else {
	window.close()
	}
}
function WBConfirmLink(action) {
	 if (checkIt(action)) {
		 if (action[2] != "(Empty Reference!)" && action[2] != "(EmptyReference!)") {
		 	if (action[3].length < 1) {
				parent.location.href=action[2];
			}
			else {
				parent.frames[action[3]].location.href=action[2];
			}
		}
	}
	return;
}
function checkIt(action) {
	var carryOn = window.confirm(action[1]);
	return carryOn;
	}
function CSCSSRedirect(action) { 
if (navigator.platform.indexOf("Win32") != -1) { os = "windows" };
if (navigator.platform.indexOf("Mac") != -1) { os = "mac" };
if (navigator.platform.indexOf("x") != -1 || navigator.platform.indexOf("BSD") != -1 || navigator.platform.indexOf("S") != -1) { os = "unix" };
if (navigator.appName.indexOf("Microsoft") != -1) { browser = "IE" };
if (navigator.appName.indexOf("Netscape") != -1) { browser = "NN" };
if (navigator.appVersion.indexOf("4.") != -1) { version = 4 };
if (navigator.appVersion.indexOf("5.") != -1) { version = 5 };
var tag='<LINK REL="styleSheet" TYPE="text/css" HREF="'+action[2]+'" >';
var type=action[1];
	if(type==0 && os=="mac" && browser=="NN" && version==4) { document.write(tag) }
	else 
	if(type==1 && os=="mac" && browser=="NN" && version==5) { document.write(tag) }
	else 
	if(type==2 && os=="windows" && browser=="NN" && version==4) { document.write(tag) }
	else 
	if(type==3 && os=="windows" && browser=="NN" && version==5) { document.write(tag) }
	else 
	if(type==4 && os=="unix" && browser=="NN" && version==4) { document.write(tag) }
	else 
	if(type==5 && os=="unix" && browser=="NN" && version==5) { document.write(tag) }
	else 
	if(type==6 && os=="mac" && browser=="IE") { document.write(tag) }
	else 
	if(type==7 && os=="windows" && browser=="IE") { document.write(tag) }
}
function initArray() {
this.length = initArray.arguments.length;
for (var i = 0; i < this.length; i++)
this[i+1] = initArray.arguments[i]; 
}
function dailyRedirect(action) {
var dateArray = new
initArray("Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday");
var today = new Date();
var day = dateArray[today.getDay()];
if (today.getDay() == 0) { day = "Sunday"; }
if (day == "Monday" && action[1] != "(Empty Reference!)" && action[1] != "(EmptyReference!)") window.location = action[1]
if (day == "Tuesday" && action[2] != "(Empty Reference!)" && action[2] != "(EmptyReference!)") window.location = action[2]
if (day == "Wednesday" && action[3] != "(Empty Reference!)" && action[3] != "(EmptyReference!)") window.location = action[3]
if (day == "Thursday" && action[4] != "(Empty Reference!)" && action[4] != "(EmptyReference!)") window.location = action[4]
if (day == "Friday" && action[5] != "(Empty Reference!)" && action[5] != "(EmptyReference!)") window.location = action[5]
if (day == "Saturday" && action[6] != "(Empty Reference!)" && action[6] != "(EmptyReference!)") window.location = action[6]
if (day == "Sunday" && action[7] != "(Empty Reference!)" && action[7] != "(EmptyReference!)") window.location = action[7]
}
function frameP(action) {
		if(parent.frames.length==0) {
			var fileName=window.location.href.substring(window.location.href.lastIndexOf("/")+1,window.location.href.length);
			window.location.href=action[1]+"?"+action[2]+"="+fileName;
		} else {
			if(top.location.search!="") {
				var sFrame=top.location.search.substring(1,top.location.search.indexOf("="));
				if(name==sFrame) {
					var sName=top.location.search.substring(top.location.search.indexOf("=")+1,top.location.search.length);
					var fileName=window.location.href.substring(window.location.href.lastIndexOf("/")+1,window.location.href.length);
					if(fileName!=sName) {
						location=sName;
					}
				}
			}
		}
	}
function CSGoBack1() { history.back() }
function CSGotoLink(action) {
	if (action[2].length) {
		var hasFrame=false;
		for(i=0;i<parent.frames.length;i++) { if (parent.frames[i].name==action[2]) { hasFrame=true; break;}}
		if (hasFrame==true)
			parent.frames[action[2]].location = action[1];
		else
			window.open (action[1],action[2],"");
	}
	else location = action[1];
}
function CSKeePress(action) {
desiredKey1 = action[1];
calledAction1 = action[2];
desiredKey2 = action[3];
calledAction2 = action[4];
desiredKey3 = action[5];
calledAction3 = action[6];
desiredKey4 = action[7];
calledAction4 = action[8];
if (document.layers) { document.captureEvents(Event.KEYPRESS) ; }
document.onkeydown = checkKey;
function checkKey(e) {
	e = (e) ? e : (window.event) ? window.event : "";
	if (e) { var keyPressed = ""
			 if (e.which)  keyPressed = e.which; 
			 else  keyPressed = event.keyCode;
			 alphaNum = String.fromCharCode(keyPressed).toLowerCase();
			 if (desiredKey1 == alphaNum) { CSAction(new Array(calledAction1)) };
			 if (desiredKey2 == alphaNum) { CSAction(new Array(calledAction2)) };
			 if (desiredKey3 == alphaNum) { CSAction(new Array(calledAction3)) };
			 if (desiredKey4 == alphaNum) { CSAction(new Array(calledAction4)) };
	}
}
checkKey();
}
function CSPAKkillframe() { 
if (self.parent.frames.length != 0)
self.parent.location = document.location
}
function CSHistoryGo(action) { history.go(action[1]); }
function CSOpenWindowPrompt(action) {
var where = self.location.href
var nw = prompt("Enter a new width:","")
if (nw != null) {
var nh = prompt("Enter a new height:","")
}
var wf = "";	
wf = wf + "width=" + nw;
wf = wf + ",height=" + nh;
wf = wf + ",resizable=" + (action[1] ? "yes" : "no");
wf = wf + ",scrollbars=" + (action[2] ? "yes" : "no");
wf = wf + ",menubar=" + (action[3] ? "yes" : "no");
wf = wf + ",toolbar=" + (action[4] ? "yes" : "no");
wf = wf + ",directories=" + (action[5] ? "yes" : "no");
wf = wf + ",location=" + (action[6] ? "yes" : "no");
wf = wf + ",status=" + (action[7] ? "yes" : "no");		
	if (nw == null || nh == null) { 
	return null
	} else {
	window.open(where,'testwin',wf);
	}
}
function CSOpenWindow(action) {
	var wf = "";	
	wf = wf + "width=" + action[3];
	wf = wf + ",height=" + action[4];
	wf = wf + ",resizable=" + (action[5] ? "yes" : "no");
	wf = wf + ",scrollbars=" + (action[6] ? "yes" : "no");
	wf = wf + ",menubar=" + (action[7] ? "yes" : "no");
	wf = wf + ",toolbar=" + (action[8] ? "yes" : "no");
	wf = wf + ",directories=" + (action[9] ? "yes" : "no");
	wf = wf + ",location=" + (action[10] ? "yes" : "no");
	wf = wf + ",status=" + (action[11] ? "yes" : "no");		
	window.open(action[1],action[2],wf);
}
function CSPDFredirect(action) {    
    if(navigator.mimeTypes && navigator.mimeTypes["application/pdf"] && navigator.mimeTypes["application/pdf"].enabledPlugin) {
		location.href=action[1]
	}
    else if (navigator.appName == "Microsoft Internet Explorer") {
       	x = confirm("The page you are trying to view requires the Adobe Portable Document Format (.PDF) browser plug-in or the Adobe Acrobat Reader application. We could not detect if your browser has this plug-in installed. To attempt to view the page anyway, click OK. Otherwise click CANCEL to view an alternate page")
		if (x == true)  location.href=action[1] 
    }
	else {
		x = confirm("Your browser may not be able to display PDF files. To attempt to view the PDF page anyway, click OK. Otherwise click CANCEL to view an alternate page")
		if (x == true)  location.href=action[1] 
	}         
}
function CSRandomLinks(action){
var urlcounter = 0;
if (action[2] != "(Empty Reference!)" && action[2] != "(EmptyReference!)") { ++urlcounter};
if (action[3] != "(Empty Reference!)" && action[3] != "(EmptyReference!)") { ++urlcounter};
if (action[4] != "(Empty Reference!)" && action[4] != "(EmptyReference!)") { ++urlcounter};
if (action[5] != "(Empty Reference!)" && action[5] != "(EmptyReference!)") { ++urlcounter};
if (action[6] != "(Empty Reference!)" && action[6] != "(EmptyReference!)") { ++urlcounter};
if (action[7] != "(Empty Reference!)" && action[7] != "(EmptyReference!)") { ++urlcounter};
if (parent.frames.length > 0) {
if (action[1].length > 0) { parent.frames[action[1]].location=action[Math.floor(Math.random()*urlcounter)+2] }
}
else 
if (action[1].length > 0) { window.open(action[Math.floor(Math.random()*urlcounter)+2],action[1],"") }	
if (action[1].length == 0) { top.location=action[Math.floor(Math.random()*urlcounter)+2] }
}
function getCookie(thecookie){
	tempString= thecookie + "=";
	if(document.cookie.length>0){
		start = document.cookie.indexOf(tempString);
		if(start!=-1){
			end = document.cookie.indexOf(";",start);
			if(end==-1){end=document.cookie.length;}
			start += tempString.length;
			return unescape(document.cookie.substring(start,end))
		}
	}
}
function setCookie(thecookie,value,expire){
	cookieExpires = new Date
	cookieExpires.setMonth(cookieExpires.getMonth() + 6)
	document.cookie = thecookie + "=" + escape(value) + ";expires=" + cookieExpires.toGMTString();
}
function testCookie(thecookie){
	if(getCookie(thecookie)){
		return(getCookie(thecookie));
	}else{
		return false;
	}
}
function CSredPrompt(action) {
	var thecookie = action[5]
	if(testCookie(action[5]) && action[4] == true){	
			location.href=getCookie(action[5]);
	} else {
	var x = 0;
	}
	
	function kill() {
	alert(action[3]);
	x = 1;
	var enter = prompt(action[1],action[2]); 
	if (enter == action[6]){setCookie(action[5], action[7]);location.href=action[7];}
	else if (enter == action[8]){setCookie(action[5], action[9]);location.href=action[9];}
	else if (enter == action[10]){setCookie(action[5], action[11]);location.href=action[11];}
	else if (enter == action[12]){setCookie(action[5], action[13]);location.href=action[13];}
	else if (enter == action[15]){setCookie(action[5], action[16]);location.href=action[16];}
	else if (enter == action[2]){kill();}
	else if (enter == null) return
	else  kill()
	}
	if (x == 0) {
	var enter = prompt(action[1],action[2]); 
	if (enter == action[6]){setCookie(action[5], action[7]);location.href=action[7];}
	else if (enter == action[8]){setCookie(action[5], action[9]);location.href=action[9];}
	else if (enter == action[10]){setCookie(action[5], action[11]);location.href=action[11];}
	else if (enter == action[12]){setCookie(action[5], action[13]);location.href=action[13];}
	else if (enter == action[14]){setCookie(action[5], action[15]);location.href=action[15];}
	else if (enter == action[2]){kill();}
	else if (enter == null) return
	else  kill()
	}
}
function CSSlideNewWindow(action) {
var wh = ""
if (action[7] || action[8] || action[9] == true) wh=action[4]-125
else wh = action[4]
var wf = "";	
wf = wf + "width=" + action[3];
wf = wf + ",height=" + wh;
wf = wf + ",resizable=" + (action[5] ? "yes" : "no");
wf = wf + ",scrollbars=" + (action[6] ? "yes" : "no");
wf = wf + ",menubar=" + (action[7] ? "yes" : "no");
wf = wf + ",toolbar=" + (action[8] ? "yes" : "no");
wf = wf + ",directories=" + (action[9] ? "yes" : "no");
wf = wf + ",location=" + (action[10] ? "yes" : "no");
wf = wf + ",status=" + (action[11] ? "yes" : "no");		
if(navigator.appVersion.charAt(0) >=4) {
var sw=screen.width-20;
var sh=screen.height;
var newwidth=action[3]; 
var newheight=action[4];
var positionleft=(sw-newwidth)/2;
var positiontop=""
if (action[7] || action[8] || action[9] == true) positiontop=(sh-newheight)/3;
positiontop=(sh-newheight)/2.5;
}
newwindow=window.open(action[1],action[2],wf); 
newwindow.focus()	
if(navigator.appVersion.charAt(0) >=4) {
	for(width1 = 1 ; width1 < positionleft ; width1 = width1 + 10)
	newwindow.moveTo(width1,positiontop)
	}
}
function CSSVGredirect(action){   
	 if(navigator.mimeTypes && navigator.mimeTypes["image/svg-xml"] && navigator.mimeTypes["image/svg-xml"].enabledPlugin ) {
		location.href=action[1]
	}
	 else if (navigator.appName == "Microsoft Internet Explorer") {
       	x = confirm("The page you are trying to view requires the Adobe Scaling Vector Graphics (.SVG) browser plug-in. We could not detect if your browser has this plug-in installed. To attempt to view the page anyway, click OK. Otherwise click CANCEL to view an alternate page")
			if (x == true)  location.href=action[1] 
    }
	else if(action[2] == true) {
		alert(action[3])
	}         
}
		
function CSSWFredirect(action) {    
    if(navigator.mimeTypes && navigator.mimeTypes["application/x-shockwave-flash"] && navigator.mimeTypes["application/x-shockwave-flash"].enabledPlugin) {
		location.href=action[1]
    }
   else if (navigator.appName == "Microsoft Internet Explorer") {
       	x = confirm("The page you are trying to view requires the Macromedia Flash (.SWF) browser plug-in. We could not detect if your browser has this plug-in installed. To attempt to view the page anyway, click OK. Otherwise click CANCEL to view an alternate page")
			if (x == true)  location.href=action[1] 
	}
	else if(action[2] == true) {
		alert(action[3])
	}         
}
function CSPAKtrg2frames(action) { 
	parent.frames[action[1]].location.href = action[2]
	parent.frames[action[3]].location.href = action[4]
 }
function CSPakRemote(action) { 
	if (TRversion()) {
		if (action[2].length < 1) {
			opener.location.href=action[1];
		}
		else {
			opener.parent.frames[action[2]].location.href=action[1];
		}
	}
	return;
}
function TRversion() {
	return (navigator.appName.indexOf("Netscape") >= 0 && parseInt(navigator.appVersion.charAt(0)) >= 3)
          || (navigator.appName.indexOf("Explorer") >= 0 && parseInt(navigator.appVersion.charAt(0)) >= 3);
}
function CStextswapID(action) {
(action[2] != "") ? (updateobject = action[2]) : (updateobject = action[1]);
(action[5] == true) ? (whichfunction = "disappear()") : (whichfunction = "revert()");
if(document.all) { originaltext = document.all(updateobject).innerHTML };
if(document.getElementById) { originaltext = document.getElementById(updateobject).innerHTML};
timeout = (action[4] != "") ? (action[4] * 1000) : 0;
if(document.getElementById) { originaltext = document.getElementById(updateobject).innerHTML};
timeout = (action[4] != "") ? (action[4] * 1000) : 0;
if (document.all)	 {  document.all(updateobject).innerHTML = action[6] + action[3] + action[7]; if (timeout != 0)  setTimeout(whichfunction,timeout); } 
	else { 	if (document.getElementById) { document.getElementById(updateobject).innerHTML = action[6] + action[3] + action[7];
			if (timeout != 0)   setTimeout(whichfunction,timeout)  }
		}
}
function disappear() {  if (document.all) { document.all(updateobject).innerHTML = "<div id=updateobject></div>" } 
	else { if (document.getElementById) { document.getElementById(updateobject).innerHTML = "" }
	}
}
function revert() { if (document.all) { document.all(updateobject).innerHTML = originaltext } 
	else { if (document.getElementById) { document.getElementById(updateobject).innerHTML = originaltext }
	}
}
function timeRedirect(action) {
var now = new Date();
var hours = now.getHours();
var timeValue = action[1];
if (timeValue >= 12) { timeValue = timeValue - 12; } // deals with 24-hour time
if (action[2] == true) { timeValue += 12; } // deals with PM times
if (hours < timeValue && action[4] != "(Empty Reference!)" && action[4] != "(EmptyReference!)" && action[3] == true) {
window.location = action[4]; }
if (hours >= timeValue && action[6] != "(Empty Reference!)" && action[6] != "(EmptyReference!)" && action[5] == true) {
window.location = action[6]; }
}
function CSDocWrite(action) { document.write(action[1]); }
function CSshowdate(action) {
	form = action[1];
	elem = action[2];
	modified = new Date(document.lastModified);
	theMonth = modified.getMonth() + 1;
	theDate = modified.getDate();
	theYear = (navigator.appVersion.indexOf("3.0") != -1) ? (modified.getYear()) : (modified.getFullYear());
	theDay = modified.getDay();
	days = new Array("Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday");
	modified = new Date(document.lastModified);
	today = days[theDay];
	if (action[3] == 0) todisplay = theMonth + "/" + theDate + "/" + theYear;
	if (action[3] == 1) todisplay = theDate + "/" + theMonth + "/" + theYear;
	if (action[3] == 2) todisplay = theYear + "/" + theMonth + "/" + theDate;
	if (action[3] == 3) todisplay = theYear + "/" + theDate + "/" + theMonth;
	if (action[4] == true) todisplay = today + ", " + todisplay;
	if(form != "" || elem != "") { document.forms[form].elements[elem].value = todisplay} ;
	if (action[5] == false) { alert(todisplay) } 
		}
function CSshowdateID(action) {
(action[2] != "") ? (updateobject = action[2]) : (updateobject = action[1]);
modified = new Date(document.lastModified);
theMonth = modified.getMonth() + 1;
theDate = modified.getDate();
theYear = (navigator.appVersion.indexOf("3.0") != -1) ? (modified.getYear()) : (modified.getFullYear());
theDay = modified.getDay();
days = new Array("Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday");
today = days[theDay];
if (action[3] == 0) todisplay = theMonth + "/" + theDate + "/" + theYear;
if (action[3] == 1) todisplay = theDate + "/" + theMonth + "/" + theYear;
if (action[3] == 2) todisplay = theYear + "/" + theMonth + "/" + theDate;
if (action[3] == 3) todisplay = theYear + "/" + theDate + "/" + theMonth;
if (action[4] == true) todisplay = today + ", " + todisplay;
if (document.all) { document.all(updateobject).innerHTML = action[6] + action[5] + todisplay + action[7]
	 } else {
	if (document.getElementById) { document.getElementById(updateobject).innerHTML = action[6] + action[5] + todisplay + action[7] }
	}
}
function CSOpenAlert(action) { alert(action[1]); }
var actn1 = "";
var actn2 = "";
var pass=""
var z=23;
var y=28;
iCounter = 3;
if (Array) {
	var f= new Array();
	var K= new Array();
	var base= new Array("0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z","a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z")
} 
function inc(){
iCounter--
if (iCounter > 0)
	{
	if (confirm("\nPassword is incorrect.\n\n\n\nRetry?"))
		Check()
	} 
	else
		alert('Access Denied');
} 
function Check(){
pass = prompt("Enter your password.","")
if(pass==null || pass==""){
	alert("You did not enter a password!");
	if(pass==""){
		Check()
	} 
} 
else{
	var lpass=(pass.length)+1
	for (l=1; l<lpass; l++){
		K[l]=pass.charAt(l)
	} 
	var transmit=0;
	for (y=1; y<lpass; y++){
		for(x=0; x<62; x++){
			if (K[y]==base[x]){
				transmit+=f[x]
				transmit*=y
			} 
		} 
	} 
	if (transmit==parseInt(actn1)) 	
		go()
	else
		inc()
} 
} 
function go(){
alert(actn2);
location.href=pass+".html";
} 
function PVpassword(action) { 
if (Array) { 
	actn1 = action[1];
	actn2 = action[2];
	z=23;
	y=28;
	for (x=0; x<10; x++){
		f[x]=x<<9
		f[x]+=23
	} 
	for (x=10; x<36; x++){
		y=y<<1
		v= Math.sqrt(y)
		v = parseInt(v,16)
		v+=5
		f[x]=v
		y++
	} 
	for (x=36; x<62; x++){
		z=z<<1
		v= Math.sqrt(z)
		v = parseInt(v,16)
		v+=74
		f[x]=v
		z++
	} 
	iCounter = 3;
	Check();
} 
}
function CSSetStatus(action) { self.status = action[1]; }
var gCSIEDragObject = null;
var gDragX,gDragY;
function CSSetupDrag (layerName) {
	this.x = 0; this.y = 0;
	if (IsIE()) {
		this.canDrag=true; 
		this.layerObj=document.all.tags("div")[layerName];
		this.layerObj.dragObj = this;
		document.ondragstart = CSIEStartDrag;
		document.onmousedown = CSIEMouseDown;
		document.onmouseup = CSIEStopDrag;
	} else {
		if (CSBVers>=5)
			{
			this.layerObj=document.getElementById(layerName);
			this.layerObj.addEventListener("mousedown", CSNS6StartDrag, true);
			this.layerObj.addEventListener("mouseup", CSNS6StopDrag, true);
			}
		else
			{
			this.layer=CSNSStyl(layerName);this.onmousemove=null; 
			this.layer.document.theLayer=this;
			this.layer.document.captureEvents(Event.MOUSEDOWN);
			this.layer.document.onmousedown=CSNSStartDrag; 
			this.layer.document.onmouseup=CSNSStopDrag;
			}
	}
}
function CSNS6StartDrag (ev) {
	CSIDOM();
	ev.currentTarget.addEventListener("mousemove", CSNS6DoDrag, true);
	gDragX=ev.clientX;
	gDragY=ev.clientY;
	ev.preventDefault();
}
function CSNS6DoDrag (ev) {
	var style=ev.currentTarget.style;
	style.left = parseInt(style.left)+(ev.clientX-gDragX)+"px";
	style.top = parseInt(style.top)+(ev.clientY-gDragY)+"px";
	gDragX=ev.clientX;
	gDragY=ev.clientY;
}
function CSNS6StopDrag (ev) {	
	ev.target.removeEventListener("mousedown", CSNS6StartDrag, true);
	ev.target.removeEventListener("mouseup", CSNS6StopDrag, true);
	ev.currentTarget.removeEventListener("mousemove", CSNS6DoDrag, true);
	ev.preventDefault();
}
function CSNSStartDrag (ev) {
	var clickInMe = false;
	if (ev.target != this) {
		for (var i=0;i<this.images.length;i++) {
			if (this.images[i] == ev.target) { clickInMe = true; break;}
			}
		}
	else clickInMe = true;	
	if (clickInMe)
		{
		this.captureEvents(Event.MOUSEMOVE|Event.MOUSEUP); 
		this.onmousemove=CSNSDoDrag;
		this.theLayer.x= ev.pageX;
		this.theLayer.y= ev.pageY;
		this.routeEvent(ev);
		return false;
		}
   this.onmousemove=null;this.releaseEvents(Event.MOUSEMOVE|Event.MOUSEUP);
	this.routeEvent(ev);
   return true; 
}
function CSNSStopDrag (ev) {
   this.onmousemove=null;this.releaseEvents(Event.MOUSEMOVE|Event.MOUSEUP);return false; 
}
function CSNSDoDrag (ev) {
	this.theLayer.layer.moveBy(ev.pageX-this.theLayer.x, ev.pageY-this.theLayer.y); 
	this.theLayer.x = ev.pageX; 
	this.theLayer.y = ev.pageY;
	this.routeEvent(ev);
}
function CSIEStartDrag () {
	if(gCSIEDragObject != null && (gCSIEDragObject.tagName==event.srcElement.tagName))
		event.returnValue=false;  
}
function CSIEStopDrag () { gCSIEDragObject=null; document.onmousemove=null; }
function CSIEMouseDown () {
	if(event.button==1) {
		dragLayer = event.srcElement;
		while (dragLayer!=null) 
			{
			if ((dragLayer.dragObj == null) && (dragLayer.tagName == "DIV"))
				break;
			if (dragLayer.dragObj != null)
				break;
			dragLayer=dragLayer.parentElement;
			}
			
		if (dragLayer == null) return;
		if (dragLayer.dragObj!=null && dragLayer.dragObj.canDrag) {
			gCSIEDragObject = dragLayer;
			gCSIEDragObject.dragObj.x=event.clientX;
			gCSIEDragObject.dragObj.y=event.clientY;
			document.onmousemove = CSIEMouseMove;
		}
	}
}
function CSIEMouseMove () {
	gCSIEDragObject.dragObj.layerObj.style.pixelLeft+=(event.clientX-gCSIEDragObject.dragObj.x);
	gCSIEDragObject.dragObj.layerObj.style.pixelTop+=(event.clientY-gCSIEDragObject.dragObj.y);
	gCSIEDragObject.dragObj.x=event.clientX;
	gCSIEDragObject.dragObj.y=event.clientY;
	event.returnValue = false;
	event.cancelBubble = true;
}
var gDragArray = new Array();
function CSDrag(action) { gDragArray[gDragArray.length] = new CSSetupDrag(action[1]); }
function CSFlipMove(action) {
	if (action[1] == '') return;
	var curX = CSGetStylePos(action[1],0); var curY = CSGetStylePos(action[1],1);
	var x1 = action[2][0];
	var y1 = action[2][1];
	if ((x1 != curX) || (y1 != curY)) CSSlideLayer(action[1],action[2],action[4],action[5]);
	else CSSlideLayer(action[1],action[3],action[4],action[5]);
}
if (navigator.appName == "Netscape" && navigator.appVersion.charAt(0) >=5) {
	leftH = ".left";
	topV = ".top";
	nndoc = "document.getElementById(layername).style";
	iestyle = "";
	offsetH = "window.pageXOffset";
	offsetV = "window.pageYOffset";
	} 
	
	else if (navigator.appName == "Netscape" && navigator.appVersion.charAt(0) >=4) {
	leftH = ".left";
	topV = ".top";
	nndoc = "document.";
	iestyle = "";
	offsetH = "window.pageXOffset";
	offsetV = "window.pageYOffset";	
	} 
	
	else if (navigator.appName == "Microsoft Internet Explorer") {
	leftH = ".pixelLeft";
	topV = ".pixelTop";
	nndoc = "";
	iestyle = "document.getElementById(layername).style";
	offsetH = "document.body.scrollLeft";
	offsetV = "document.body.scrollTop";
	}
function getnewPos() {
var currentH = eval(offsetH);
var currentV = eval(offsetV);
x = parseInt(currentH) + parseInt(xpos);
y = parseInt(currentV) + parseInt(ypos);
moveLayer();
setTimeout("getnewPos()",10)
}
function CSFloatLayer(action) {
layername = action[1];
ypos = action[2];
xpos = action[3];
if (navigator.appVersion.charAt(0) >=5 || navigator.appName == "Microsoft Internet Explorer") {
	leftPos = eval(nndoc + iestyle + leftH);
	topPos = eval(nndoc + iestyle + topV);
	} else {
	leftPos = eval(nndoc + layername + iestyle + leftH);
	topPos = eval(nndoc + layername + iestyle + topV);
	}
	getnewPos()
	}
function moveLayer() {
if (navigator.appVersion.charAt(0) >=5  || navigator.appName == "Microsoft Internet Explorer") {
	eval(nndoc + iestyle + topV + "=" + y);
	eval(nndoc + iestyle + leftH + "=" + x);
	} else {
	eval(nndoc + layername + iestyle + topV + "=" + y);
	eval(nndoc + layername + iestyle + leftH + "=" + x);
	}
}
var myLayer = false;
var offSetX = 1;
var offSetY = 1;
function CSMouseFollow(action,offX,offY){
	myLayer = action[1];
	if(offX){offSetX=offX;}
	if(offY){offSetY=offY;}
	if (navigator.appName == "Netscape") {
		document.captureEvents(Event.MOUSEMOVE);
	} else {
		if(navigator.appVersion.indexOf("Mac")!=-1){offSetX=offSetX-18;}
	}
	document.onmousemove = mouseLayer;
}
function mouseLayer(e){
	if (navigator.appName == "Netscape" && navigator.appVersion.charAt(0) >=5) {
		document.getElementById(myLayer).style.left = (e.pageX+offSetX);
		document.getElementById(myLayer).style.top = (e.pageY+offSetY); 
		}
		else if (navigator.appName == "Netscape") {
			document.layers[myLayer].moveBy(
			(e.pageX+offSetX) - document.layers[myLayer].left, 
			(e.pageY+offSetY) - document.layers[myLayer].top);
	   		 return true; 
		} else {
		document.all[myLayer].style.pixelLeft = (event.x+offSetX);
		document.all[myLayer].style.pixelTop = (event.y+offSetY);
		event.cancelBubble = true;
		event.returnValue = false;
	}
}
function CSMoveBy(action)
{
	x = CSGetStylePos(action[1], 0);
	y = CSGetStylePos(action[1], 1);
	x += parseInt(action[2]);
	y += parseInt(action[3]);
	x = CSSetStylePos(action[1], 0, x);
	y = CSSetStylePos(action[1], 1, y);
}
function CSMoveTo(action) { CSSlideLayer(action[1],action[2],action[3],action[4]); }
function CSPlayScene(action) { CSStartSeq (action[1]); }
var CSLastSound = null
function CSPlaySound(action) {
	if (eval('document.'+action[1])!=null) {
		if (CSLastSound != null && CSLastSound != action[1]) eval ('document.' + CSLastSound + '.stop()');
		CSLastSound = action[1]
		if (window.navigator.userAgent.indexOf("MSIE") > 0) eval ('document.' + CSLastSound + '.run()');
		else eval ('document.' + CSLastSound + '.play(true)');
	} else { alert ("The current Plug-In cannot be controlled by JavaScript. Please try using LiveAudio or a compatible Plug-In!"); }
}
function CSShowHide(action) {
	if (action[1] == '') return;
	var type=action[2];
	if(type==0) CSSetStyleVis(action[1],0);
	else if(type==1) CSSetStyleVis(action[1],1);
	else if(type==2) { 
		if (CSGetStyleVis(action[1]) == 0) CSSetStyleVis(action[1],1);
		else CSSetStyleVis(action[1],0);
	}
}
SSnumimg=1; SSsens2=-1;SSsens3=-1
function CSSlideShow(action) 
{
SSmax=action[2]
SSimgNom=action[1]
SSloop=action[4]
SSsens=action[3] 
SSpalin=action[5]
var SSimg = null;
	if (document.images) {
		if (!IsIE()&&CSBVers<5) SSimg = CSFindElement(SSimgNom,0);
		else SSimg = document.images[SSimgNom];
SSstr=SSimg.src
SSn=SSstr.length
SSp=SSn-6
SSpstr=SSstr.substring(0,SSp)
SSnimg=SSstr.substring(SSp,SSp+2)
SSformat=SSstr.substring(SSp+2,SSn)
if (SSformat==".jpg" || SSformat==".JPG" || SSformat==".gif" || SSformat==".GIF")
{}
else
{
 alert("Image extension must be .jpg or .gif (case sensitive). Images must be numbered 01, 02 ...")
}
slide(SSmax,SSformat,SSpstr,SSnimg,SSimgNom,SSloop,SSpalin)
}
}
function slide(SSmax,SSformat,SSpstr,SSnimg,SSimgNom,SSloop,SSpalin)
{
if (SSsens2==true) {SSsens=true}
if (SSsens2==false) {SSsens=false}
if (SSsens==true) 
{
SSsuite=SSnumimg-1
	if (SSnumimg>SSmax)SSsuite=SSmax
	if (SSnumimg<=1 & SSloop==true & SSpalin!=true) { SSsuite=SSmax }
	if (SSnumimg<=1 & SSloop==true & SSpalin==true) { 
		if (SSsens2==-1 & SSsens3==-1) {SSsuite=SSmax;SSsens3=1} else { SSsuite=SSnumimg+1; SSsens2=false }}
	if (SSnumimg<=1 & SSloop!=true & SSpalin!=true) {
		if  (SSsens2==-1 & SSsens3==-1) { SSsuite=SSmax;SSsens3=1 } else {SSsuite=SSnumimg; SSfini()}}
}
else
{
SSmax=SSmax-1
SSsuite=SSnumimg+1
	if (SSnumimg>SSmax & SSloop==true & SSpalin!=true) { SSsuite=1}
	if (SSnumimg>SSmax & SSloop==true & SSpalin==true) {SSsuite=SSnumimg-1; SSsens2=true }
	if (SSnumimg>SSmax & SSloop!=true &  SSpalin!=true) { SSsuite=SSnumimg;SSfini() }
	if (SSnumimg<1) SSsuite=1
}
SSnumimg=SSsuite
if (SSsuite<10) {
	SSaller="0"+SSsuite
	}
	else SSaller=SSsuite
SSsource=SSpstr+SSaller+SSformat
var SSimg = null;
	if (document.images) {
		if (!IsIE()&&CSBVers<5) SSimg = CSFindElement(SSimgNom,0);
		else SSimg = document.images[SSimgNom];
		if (SSimg) SSimg.src = SSsource;
	}
}
function SSfini() {
}
function CSSlideShowAuto(action) 
{
SSAfini=0
SSAnumimg=0
SSAmax=action[2]
SSAimgNom=action[1]
SSAtemps=action[3]*1000
if (action[4]==true) 
		{
		SSAstop=true
		}
	else SSAstop=false
var SSAimg = null;
	if (document.images) {
		if (!IsIE()&&CSBVers<5) SSAimg = CSFindElement(SSAimgNom,0);
		else SSAimg = document.images[SSAimgNom];
str=SSAimg.src
n=str.length
p=n-6
SSApstr=str.substring(0,p)
SSAnimg=str.substring(p,p+2)
SSAformat=str.substring(p+2,n)
if (SSAformat==".jpg" || SSAformat==".JPG" || SSAformat==".gif" || SSAformat==".GIF")
{}
else
{
 alert("Image extension must be .jpg or .gif (case sensitive). Images must use 2 digit naming starting with 01, 02 ... plus extension")
}
if (SSAnimg.substring(0,1)=="0") 
{
SSAnumimg=Number(SSAnimg.substring(1,2))
}
else
{SSAnumimg=Number(SSAnimg)}
SSAtempo(SSAmax,SSAimgNom,SSAtemps,SSAstop,SSApstr,SSAnimg,SSAformat)
}
}
function SSAtempo(SSAmax,SSAimgNom,SSAtemps,SSAstop,SSApstr,SSAnimg,SSAformat)
{
setTimeout("slideAuto(SSAmax,SSAimgNom,SSAstop,SSApstr,SSAnimg,SSAformat)",SSAtemps)
}
function slideAuto(SSAmax,SSAimgNom,SSAstop,SSApstr,SSAnimg,SSAformat)
{
if (SSAfini==1) {
SSAnumimg = SSAnumimg-2
CSSlideShowAutoPause()
}
else 
{
SSAmax=SSAmax-1
SSAsuite=SSAnumimg+1
	if (SSAnumimg>SSAmax)
		{
		SSAsuite=1
		if (SSAstop==true) SSAfini=1
		else
		SSAfini=0
		}
	if (SSAnumimg<1) SSAsuite=1
SSAnumimg=SSAsuite
if (SSAsuite<10) {
	SSAaller="0"+SSAsuite
	}
	else SSAaller=SSAsuite
SSAsource=SSApstr+SSAaller+SSAformat
var SSAimg = null;
	if (document.images) {
		if (!IsIE()&&CSBVers<5) SSAimg = CSFindElement(SSAimgNom,0);
		else SSAimg = document.images[SSAimgNom];
		if (SSAimg) SSAimg.src = SSAsource;
	}
SSAtempo(SSAmax,SSAimgNom,SSAtemps,SSAstop,SSApstr,SSAnimg,SSAformat)
}
}
function CSSlideShowAutoPause() 
{}
function CSSlideShowAutoStop(action) 
{
if (SSAfini==0) SSAfini=1
else SSAfini=0 ; SSAnumimg = SSAnumimg+2 ;  slideAuto(SSAmax,SSAimgNom,SSAstop,SSApstr,SSAnimg,SSAformat)
}
function CSStopAll(action) { CSStopComplete (); }
function CSStopScene(action) { CSStopFunction (action[1]); }
function CSStopSound (action) {if (eval('document.'+action[1])!=null) { eval ('document.' + action[1] + '.stop()');}}
function CSStartWipe (action)
{
	var el=CSCreateTransElement (action[1], action[2]);
	if (el==null) return;
	var dir=action[3];
	if (dir=="_inLeft") {el.steps=el.clipRect.width/el.maxValue; el.modus="in";}
	else if (dir=="_inRight") {el.steps=el.clipRect.width/el.maxValue; el.modus="in";}
	else if (dir=="_outLeft") {el.steps=el.clipRect.width/el.maxValue; el.modus="out";}
	else if (dir=="_outRight") {el.steps=el.clipRect.width/el.maxValue; el.modus="out";}
	else if (dir=="_inTop") {el.steps=el.clipRect.height/el.maxValue; el.modus="in";}
	else if (dir=="_inBottom") {el.steps=el.clipRect.height/el.maxValue; el.modus="in";}
	else if (dir=="_outTop") {el.steps=el.clipRect.height/el.maxValue; el.modus="out";}
	else if (dir=="_outBottom") {el.steps=el.clipRect.height/el.maxValue; el.modus="out";}
	else if (dir=="_inCenter") {el.HSteps=el.clipRect.width/el.maxValue; el.VSteps=el.clipRect.height/el.maxValue; el.modus="in";}
	else if (dir=="_outCenter") {el.HSteps=el.clipRect.width/el.maxValue; el.VSteps=el.clipRect.height/el.maxValue; el.modus="out";}
	if (el.modus=="") return;
	el.currentValue=0;
	el.glDir=action[3];
	CSStartFunction(CSDoWipe,el);
}
function CSDoWipe (info)
{
	var el = info.data;
	if (el==null) return false;
	if (el.currentValue==el.maxValue) { CSFinishWipe(el); return false; }
	var r = new CSRect(el.clipRect.left,el.clipRect.top,el.clipRect.width,el.clipRect.height);
	var dir=el.glDir;
	if (dir=="_inLeft") {r.left=r.width-el.currentValue*el.steps;}
	else if (dir=="_inTop") {r.top=r.height-el.currentValue*el.steps;}
	else if (dir=="_inRight") {r.width=el.currentValue*el.steps;}
	else if (dir=="_inBottom") {r.height=el.currentValue*el.steps;}
	else if (dir=="_outLeft") {r.width=r.width-el.currentValue*el.steps;}
	else if (dir=="_outTop") {r.height=r.height-el.currentValue*el.steps;}
	else if (dir=="_outRight") {r.left=el.currentValue*el.steps;}
	else if (dir=="_outBottom") {r.top=el.currentValue*el.steps;}
	else if (dir=="_inCenter") {r=CSCenterRectIn(el,r);}
	else if (dir=="_outCenter") {r=CSCenterRectOut(el,r);}
	CSSetLayerClip(el,r);
	el.currentValue+=1;
	return true;
}
function CSFinishWipe (el)
{
	if (el.modus=="in") CSSetLayerClip(el,el.clipRect);
	else { 
		el.clipRect=new CSRect(0,0,el.width,el.height); 
		CSSetLayerClip(el,el.clipRect); 
		CSSetStyleVis(el.layer,0);
	}
	CSDisposeTransElement(el);
}
function CSCenterRectIn(el,r)
{
	var hValue= el.currentValue*el.HSteps/2;
	var vValue= el.currentValue*el.VSteps/2;
	r.left=Math.round(r.left+r.width/2-hValue); 
	r.top=Math.round(r.top+r.height/2-vValue); 
	r.width=Math.round(hValue*2);
	r.height=Math.round(vValue*2);
	return r;
}
function CSCenterRectOut(el,r)
{
	var hValue= el.currentValue*el.HSteps/2;
	var vValue= el.currentValue*el.VSteps/2;
	r.left+=Math.round(hValue); 
	r.top+=Math.round(vValue); 
	r.width-=Math.round(hValue*2);
	r.height-=Math.round(vValue*2);
	return r;
}
function CSshowtimedateID(action) {
	(action[2] != "") ? (updateobject = action[2]) : (updateobject = action[1])
	showtime = action[3]
	usemilitary = action[4]
	showdate = action[5]
	dateformat = action[6]
	showday = action[7]
	leadin = action[8]
	opentag = action[9]
	closetag = action[10]
	theTimer = setTimeout("theClockDate()",10);
}
function theClockDate() {   
	now = new Date()
	theMonth = now.getMonth() + 1
	theDate = now.getDate()
	theYear = now.getFullYear()
	theDay = now.getDay()
	hours = now.getHours()
	if(navigator.appVersion.indexOf("MSIE 5.0; Macintosh") != -1) {
	if (theMonth <= 11 && theMonth >= 4) { hours = hours + 1}
	}
	ampm = ((hours >= 12) ? "PM" : "AM");
	hours = ((hours > 12 && usemilitary == false) ? hours -12 : hours);
	if (hours == 0) hours = 12;
	if (hours >= 1 && hours <=9) hours = "0" + hours;
	minutes = now.getMinutes()
	if (minutes < 10) minutes = "0" + minutes;
	time = "";
	(showtime == true) ? (time = " " + hours + ":" + minutes) : (time = "",ampm="");
	(usemilitary == true) ? (ampm="") : (ampm=ampm);
	days = new Array("Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday");
	today = days[theDay];
	if (showdate == true) {
		if (dateformat == 0) todisplay = theMonth + "/" + theDate + "/" + theYear;
		if (dateformat == 1) todisplay = theDate + "/" + theMonth + "/" + theYear;
		if (dateformat == 2) todisplay = theYear + "/" + theMonth + "/" + theDate;
		if (dateformat == 3) todisplay = theYear + "/" + theDate + "/" + theMonth;
		} else {todisplay = "" }
	if (showday == true) todisplay =  today + ", " + todisplay;
	if (document.all) { document.all(updateobject).innerHTML = opentag + leadin + todisplay + time + " " + ampm + closetag; } 
	else {
		if (document.getElementById) { document.getElementById(updateobject).innerHTML = opentag + leadin + todisplay + time + " " + ampm + closetag; }
}
theTimer = setTimeout("theClockDate()",2000);
}
function CSdigitalclock(action){
img1 = CSGetImage(action[1]);
img2 = CSGetImage(action[2]);
img3 = CSGetImage(action[3]);
img4 = CSGetImage(action[4]);
usemilitary = action[5];
giflocation = action[6];
path = giflocation.substring(0, giflocation.length - 5);
extension = (giflocation.indexOf("jpg") != -1) ? (".jpg") : (".gif");
digClock();
}
function digClock() {   
now = new Date();
hours = now.getHours();
theMonth = now.getMonth();
if(navigator.appVersion.indexOf("MSIE 5.0; Macintosh") != -1) {
	if (theMonth <= 11 && theMonth >= 4) { hours = hours + 1 };
	if (hours == 24) { hours = 0 };
	}
minutes = now.getMinutes();
hours = ((hours > 12 && usemilitary == false) ? hours -12 : hours);
if (hours >= 0 && hours <=9) hours = "0" + hours;
if (usemilitary == false && hours == 00) { hours = 12 };
if (minutes >= 0 && minutes <=9) minutes = "0" + minutes;
hours = "" + hours;
minutes = "" + minutes;
hour1 = hours.substring(0,1);
hour2 = hours.substring(1,2);
minutes1 = minutes.substring(0,1);
minutes2 = minutes.substring(1,2);
updater();
}
	
function updater() {
img1.src = path + hour1 + extension;
img2.src = path + hour2 + extension;
img3.src = path + minutes1 + extension;
img4.src = path + minutes2 + extension;
theTimer = setTimeout("digClock()",1000);
}
function CSFixFct() {
	var d = document; var w = window;
	if (d.cs.csFix.w != w.innerWidth || d.cs.csFix.h != w.innerHeight) {
		d.location = d.location; }
}
function CSNSFix(action) { 
	var d = document; var w = window;
	if ((navigator.appName == 'Netscape') && (parseInt(navigator.appVersion) == 4)) {
		if (typeof d.cs == 'undefined') { 
			d.cs = new Object;
			d.cs.csFix = new Object; 
		} else if (CSIsFrame (w) == true) CSFixFct();
		d.cs.csFix.w = w.innerWidth;
		d.cs.csFix.h = w.innerHeight; 
		window.onresize = CSFixFct;
	  }
}
function CSIsFrame (window) {
	var rootWindow = window.parent;
	if (rootWindow == 'undefined') return false;
	for (i = 0; i < rootWindow.frames.length; i++)
		if (window == rootWindow.frames[i]) return true;
	return false;
}
function CSprintdocument(action){
if (navigator.appName .indexOf("Microsoft") != -1 && navigator.platform.indexOf("Mac") != -1 || navigator.appVersion.indexOf("3.0") != -1) {
alert("Please use your browser\'s print command to print this document.");
	} else {
	(action[1] != "") ? (parent.frames[action[1]].print()) : (parent.print());
	}
}
function CSResizeWindow(action) { 
	if(navigator.appVersion.charAt(0) >=4) { window.resizeTo (action[1],action[2]) }
}
function CSScrollDown(action){
	if(navigator.appVersion.charAt(0) >=4) {
		var container = 0	
		if (action[2] > 0)		{
			while (container < action[1]) {
   				window.scrollBy(0,action[2]);
   				container = container + action[2];  
			} 	
      	}
	}
}
function CSScrollLeft(action){
	if(navigator.appVersion.charAt(0) >=4) {
		var container = 0	
		if (action[2] > 0)		{
			while (container < action[1]) {
   				window.scrollBy(-action[2],0);
   				container = container + action[2];  
			} 	
      	}
	}
}
function CSScrollRight(action){
	if(navigator.appVersion.charAt(0) >=4) {
		var container = 0	
		if (action[2] > 0)		{
			while (container < action[1]) {
   				window.scrollBy(action[2],0);
   				container = container + action[2];  
			} 	
      	}
	}
}
function CSScrollstatus(action) {
phrase = action[1];
speed = action[2];	
(action[3] != true)	? delay = setTimeout("CSScrollleftidle()",20) : delay = setTimeout("CSScrollrightidle()",20)
var thestring
leftphrase = "                                                                                                 " + phrase 
charnum = leftphrase.length;
stringnum = leftphrase.length;	
size = leftphrase.length+1
}
function CSScrollrightidle() {
	stringnum--;
	(stringnum != 0) ? window.status = leftphrase.substring(charnum,stringnum) : stringnum = charnum
	delay = setTimeout("CSScrollrightidle()",speed); 
	}
	
function CSScrollleftidle() {
   thestring = leftphrase.substring(0,1);
   leftphrase += thestring
   leftphrase = leftphrase.substring(1,size);
   window.status = leftphrase.substring(0,size);
   delay = setTimeout("CSScrollleftidle()",speed);
}
function CSScrollUp(action){
	if(navigator.appVersion.charAt(0) >=4) {
		var container = 0	
		if (action[2] > 0)		{
			while (container < action[1]) {
   				window.scrollBy(0,-action[2]);
   				container = container + action[2];  
			} 	
      	}
	}
}
function CSSearchEngine(action) {
var form = action[1]
var elem = action[2]
var theEntry  
if(action[3]==true) { theEntry = action[4] } 
else { theEntry = document.forms[form].elements[elem].value }
var type=action[5];
if(type==0) engine="http://search.adobe.com/cgi-bin/query?mss=simple&pg=q&what=web&fmt=.&where=www_search_main&superq="+theEntry+"&rd=all&q="+theEntry+"&x=39&y=10" //Adobe.com
else if(type==1) engine="http://www.altavista.digital.com/cgi-bin/query?pg=q&what=web&fmt=.&q="+theEntry //Altavista   
else if(type==2) engine="http://groups.google.com/groups?q="+theEntry+"&hl=en&lr=&safe=off&btnG=Google+Search&site=groups"; //Google Deja News (Usenet)  
else if(type==3) engine="http://www.excite.com/search.gw?trace=a&search="+theEntry; //Excite
else if(type==4) engine="http://www.search.hotbot.com/hResult.html?SM=MC&MT="+theEntry+"&DV=7&RG=.com&DC=10&DE=2&OPs=MDRTP&_v=2&DU=days&SW=web&search.x=23&search.y=8"; //HotBot  
else if(type==5) engine="http://www.google.com/search?q="+theEntry+"&btnG=Google+Search" //Google
else if(type==6) engine="http://search.msn.com/results.asp?RS=CHECKED&FORM=MSNH&v=1&q="+theEntry; //MSN
else if(type==7) engine="http://www.lycos.com/cgi-bin/pursuit?query="+theEntry+"&matchmode=and&cat=lycos&x=33&y=10"; //Lycos
else if(type==8) engine="http://shopper.cnet.com/shopping/search/results/1,10214,0-1257,00.html?tag=st%2Esh%2E1257%2Esbsr&qt="+theEntry+"&cn=&ca=1257" //Shopper.com
else if(type==9) engine="http://search.excite.com/search.gw?c=web&lk=webcrawler&onload=&s="+theEntry;//Webcrawler
else if(type==10) engine="http://search.yahoo.com/bin/search?p="+theEntry //Yahoo
if (theEntry=="") { alert("Please enter a search keyword!") } 
else  { newWindow=window.open(engine, action[6],"toolbar,location,directories,status,menubar,scrollbars,resizable=1") }
}
		
function CSSetBackColor(action) { document.bgColor = action[1]; }
function CSshowtime(action) { 
message = action[1]
military = action[2]
offset = action[3] 
dst = action[4]
statuscheck = action[5]
formcheck = action[6]
form = action[7];
elem = action[8];
var dstoffset = 0
theTimer = setTimeout("theClock()",10);
} 
    
function theClock() {   
clearTimeout(theTimer);
var now = new Date();
var correct = now.toGMTString();
time_string = correct.split(' ');
hm = time_string[4];
hm_string = hm.split(':');
var hours = hm_string[0] - 0;
var suffix = " A.M."
if (dst == true) dstoffset = 1
hours = hours + offset + dst;
	if (hours < 0) hours +=24	
	if (hours > 11 && hours < 24)  suffix = " P.M."
	if (hours > 12 && military == false) hours -=12;
	
var minutes = now.getMinutes();
var timeValue12 = "" + ((hours > 12) ? hours -12 : hours)
if (timeValue12 == 0) timeValue12 = 12
var timeValue24 = "" + ((hours > 23) ? hours -24 : hours)
timeValue12 += ((minutes < 10) ? ":0" : ":") + minutes
timeValue12 += suffix
timeValue24 += ((minutes < 10) ? ":0" : ":") + minutes
	if(military == false)  {
	WorldTime = message + timeValue12;
	} else { 
	WorldTime = message + timeValue24
	}  
	if(statuscheck == true)  window.status = WorldTime
	if(formcheck == true)  document.forms[form].elements[elem].value  = WorldTime
theTimer = setTimeout("theClock()",5000);
}
function CSActionGroup (action) {
	for(var i=1;i<action.length;i++) { CSAction(new Array(action[i])); }
}
function CSCallAction(action)
{
	CSAction(new Array(action[1]));
}
function CSCallFunction(action)
{
	var str = action[1];
	str += "(";
	str += action[2];
	str += ");"
	return eval(str);
}
function CSConditionAction(action) {
	if (action[1]) {
		if (CSAction(new Array(action[1])) == true) {
			if (action[2]) CSAction(new Array(action[2]));
		} else if (action[3]) CSAction(new Array(action[3]));
	}
}
function CSIdleObject (action) {
	this.conditionAction = action[2];
	this.trueAction = action[3];
	this.falseAction = action[4];
	this.exitIdleIfTrue = action[1];
	this.lastState = false;
}
function CSIdleAction(action) {
	idleObj = new CSIdleObject (action);
	CSStartFunction (CSDoIdle,idleObj);
}
function CSDoIdle (param) {
	idleObject=param.data;
	if (idleObject.conditionAction) {
		gCurrentIdleObject = idleObject;
		var result = CSAction(new Array(idleObject.conditionAction));
		if (result == true && idleObject.lastState==false) {
			idleObject.lastState = result;
			if (idleObject.trueAction) {
				CSAction(new Array(idleObject.trueAction));
				if (idleObject.exitIdleIfTrue == true) return false;
			}
		} else if (result == false && idleObject.lastState == true) {
			idleObject.lastState = false;
			if (idleObject.falseAction) {
				CSAction(new Array(idleObject.falseAction));
			}		
		}
	}
	return true;
}
function CSLayerIntersect (condition)
{
	var l1,t1,r1,b1,l2,t2,r2,b2;
	if (IsIE()) {
		var layer1=document.all.tags("div")[condition[1]];
		var layer2=document.all.tags("div")[condition[2]];
		l1=layer1.style.pixelLeft; t1=layer1.style.pixelTop; r1=layer1.offsetWidth+l1; b1=layer1.offsetHeight+t1;
		l2=layer2.style.pixelLeft; t2=layer2.style.pixelTop; r2=layer2.offsetWidth+l2; b2=layer2.offsetHeight+t2;	
	} else {
		var layer1=CSNSStyl(condition[1]);
		var layer2=CSNSStyl(condition[2]);
		if (CSBVers>=5)
			{
			CSIDOM();
			l1=parseInt(layer1.left); t1=parseInt(layer1.top); r1=parseInt(layer1.width)+l1; b1=parseInt(layer1.height)+t1;
			l2=parseInt(layer2.left); t2=parseInt(layer2.top); r2=parseInt(layer2.width)+l2; b2=parseInt(layer2.height)+t2;
			}
		else
			{
			l1=layer1.x; t1=layer1.y; r1=layer1.clip.width+l1; b1=layer1.clip.height+t1;
			l2=layer2.x; t2=layer2.y; r2=layer2.clip.width+l2; b2=layer2.clip.height+t2;
			}
	}
	var w = (r1 < r2 ? r1 : r2) - (l1 > l2 ? l1 : l2)
	var h = (b1 < b2 ? b1 : b2) - (t1 > t2 ? t1 : t2)
	return ((w >= 0) && (h >= 0));
}
CSCurrentPressedKey = -1;
function CSKeyPress(ev) {
	var code;
	if(IsIE()) CSCurrentPressedKey = event.keyCode;
	else CSCurrentPressedKey = ev.which;
}
document.onkeypress	= CSKeyPress;
function CSKeyCompare(condition)
{
	var eq = (condition[1] == CSCurrentPressedKey);
	if(eq)
		CSCurrentPressedKey = -1;
	return eq;
}
function CSTimeout (condition) {
	var result = false;
	if (typeof (gCurrentIdleObject) == "undefined")	return result;
	if (gCurrentIdleObject.lastTime) {
		var t=new Date();
		if (t.getTime() >= gCurrentIdleObject.lastTime) { 
			if (t.getTime() >= gCurrentIdleObject.nextTime) { 
				gCurrentIdleObject.lastTime = t.getTime() + condition[1]*1000;
				gCurrentIdleObject.nextTime = gCurrentIdleObject.lastTime + condition[1]*1000;
				return false;
			}
			return true;
		}
	} else { 
		var t=new Date();
		gCurrentIdleObject.lastTime = t.getTime() + condition[1]*1000;
		gCurrentIdleObject.nextTime = gCurrentIdleObject.lastTime + condition[1]*1000;
	}
	return result;
}
function CSDeleteCookie(action) 
{
var name=action[1]
var value=action[2]
var jours=-12000
path="/"
domain=null
var expdate = new Date ();
expdate.setTime (expdate.getTime() + (jours * 60 * 60 * 1000));
SetCookie(name,value,expdate)
}
function SetCookie (name, value) {
  var argv = SetCookie.arguments;
  var argc = SetCookie.arguments.length;
  var expires = (argc > 2) ? argv[2] : null;
  var secure = (argc > 5) ? argv[5] : false;
  document.cookie = name + "=" + escape (value) +
    ((expires == null) ? "" : ("; expires=" + expires.toGMTString())) +
    ((path == null) ? "" : ("; path=" + path)) +
    ((domain == null) ? "" : ("; domain=" + domain)) +
    ((secure == true) ? "; secure" : "");
}
function CSvisits(action) {
mycookie = action[8]
cookieExpires = new Date
cookieExpires.setMonth(cookieExpires.getMonth() + 12)
visits = eval(cookieVal(mycookie))
visits++
document.cookie = mycookie+"="+visits+";expires=" + cookieExpires.toGMTString()
                
function cookieVal(cookieName) {
	thisCookie = document.cookie.split("; ")
		for (i=0; i<thisCookie.length; i++) {
			if (cookieName == thisCookie[i].split("=")[0]) {
			return thisCookie[i].split("=")[1]
			}
	}
 return 0
}
	if(visits == 1 && action[1] == true) alert(action[2])
	else
	if(action[5] == true) alert(action[3] + " " + visits)
	if(action[6] == true) {
		if(visits == action[7])
		alert(action[4])
		}
}
		
function CSVisitorCookie(action) 
{
resultat = "visitor"
cookiename = action[1]
goUrl = action[2]
var arg = cookiename + "=";
  var alen = arg.length;
  var clen = document.cookie.length;
  var i = 0;
  while (i < clen) {
    var j = i + alen;
	   if (document.cookie.substring(i, j) == arg)
     return CSVisitorGetCookie (j);
    i = document.cookie.indexOf(" ", i) + 1;
    if (i == 0) break; 
  }
  VisitorSetCookie(cookiename)
  return null; 
}
function CSVisitorGetCookie (offset) {
  var endstr = document.cookie.indexOf (";", offset);
  if (endstr == -1) 
    endstr = document.cookie.length;
  valeur=unescape(document.cookie.substring(offset, endstr))
  if (valeur==resultat)
  VisitorGotoLink(goUrl)
  else
  VisitorSetCookie(cookiename)
}
function VisitorGotoLink(goUrl) {
location = goUrl
}
function VisitorSetCookie(cookiename) 
{
var value="visitor"
var jours=500*24
path="/"
domain=null
var expdate = new Date ();
expdate.setTime (expdate.getTime() + (jours * 60 * 60 * 1000));
SetCookie(cookiename,value,expdate)
}
function SetCookie (cookiename, value) {
  var argv = SetCookie.arguments;
  var argc = SetCookie.arguments.length;
  var expires = (argc > 2) ? argv[2] : null;
  var secure = (argc > 5) ? argv[5] : false;
  document.cookie = cookiename + "=" + escape (value) +
    ((expires == null) ? "" : ("; expires=" + expires.toGMTString())) +
    ((path == null) ? "" : ("; path=" + path)) +
    ((domain == null) ? "" : ("; domain=" + domain)) +
    ((secure == true) ? "; secure" : "");
}
// EOF
