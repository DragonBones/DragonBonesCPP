/**
 * @module dragonbones_bindings
 */
var db = db || {};

/**
 * @class DragonBonesData
 */
db.DragonBonesData = {

/**
 * @method dispose
 */
dispose : function (
)
{
},

/**
 * @method getArmatureData
 * @param {String} arg0
 * @return {dragonBones::ArmatureData}
 */
getArmatureData : function (
str 
)
{
    return dragonBones::ArmatureData;
},

/**
 * @method DragonBonesData
 * @constructor
 */
DragonBonesData : function (
)
{
},

};

/**
 * @class ITextureAtlas
 */
db.ITextureAtlas = {

/**
 * @method dispose
 */
dispose : function (
)
{
},

};

/**
 * @class Animation
 */
db.Animation = {

/**
 * @method getAnimationList
 * @return {Array}
 */
getAnimationList : function (
)
{
    return new Array();
},

/**
 * @method setTimeScale
 * @param {float} arg0
 */
setTimeScale : function (
float 
)
{
},

/**
 * @method stop
 */
stop : function (
)
{
},

/**
 * @method clear
 */
clear : function (
)
{
},

/**
 * @method dispose
 */
dispose : function (
)
{
},

/**
 * @method play
 */
play : function (
)
{
},

/**
 * @method hasAnimation
 * @param {String} arg0
 * @return {bool}
 */
hasAnimation : function (
str 
)
{
    return false;
},

/**
 * @method getLastAnimationState
 * @return {dragonBones::AnimationState}
 */
getLastAnimationState : function (
)
{
    return dragonBones::AnimationState;
},

/**
 * @method gotoAndPlay
 * @param {String} arg0
 * @param {float} arg1
 * @param {float} arg2
 * @param {int} arg3
 * @param {int} arg4
 * @param {String} arg5
 * @param {dragonBones::Animation::AnimationFadeOutMode} arg6
 * @param {bool} arg7
 * @param {bool} arg8
 * @return {dragonBones::AnimationState}
 */
gotoAndPlay : function (
str, 
float, 
float, 
int, 
int, 
str, 
animationfadeoutmode, 
bool, 
bool 
)
{
    return dragonBones::AnimationState;
},

/**
 * @method getState
 * @param {String} arg0
 * @param {int} arg1
 * @return {dragonBones::AnimationState}
 */
getState : function (
str, 
int 
)
{
    return dragonBones::AnimationState;
},

/**
 * @method getIsComplete
 * @return {bool}
 */
getIsComplete : function (
)
{
    return false;
},

/**
 * @method advanceTime
 * @param {float} arg0
 */
advanceTime : function (
float 
)
{
},

/**
 * @method getIsPlaying
 * @return {bool}
 */
getIsPlaying : function (
)
{
    return false;
},

/**
 * @method gotoAndStop
 * @param {String} arg0
 * @param {float} arg1
 * @param {float} arg2
 * @param {float} arg3
 * @param {float} arg4
 * @param {int} arg5
 * @param {String} arg6
 * @param {dragonBones::Animation::AnimationFadeOutMode} arg7
 * @return {dragonBones::AnimationState}
 */
gotoAndStop : function (
str, 
float, 
float, 
float, 
float, 
int, 
str, 
animationfadeoutmode 
)
{
    return dragonBones::AnimationState;
},

/**
 * @method getTimeScale
 * @return {float}
 */
getTimeScale : function (
)
{
    return 0;
},

/**
 * @method Animation
 * @constructor
 */
Animation : function (
)
{
},

};

/**
 * @class DBCCArmature
 */
db.CCArmature = {

/**
 * @method getCCBoundingBox
 * @return {rect_object}
 */
getCCBoundingBox : function (
)
{
    return cc.Rect;
},

/**
 * @method getCCEventDispatcher
 * @return {cc.EventDispatcher}
 */
getCCEventDispatcher : function (
)
{
    return cc.EventDispatcher;
},

/**
 * @method dispose
 */
dispose : function (
)
{
},

/**
 * @method getCCSlot
 * @param {String} arg0
 * @return {dragonBones::DBCCSlot}
 */
getCCSlot : function (
str 
)
{
    return dragonBones::DBCCSlot;
},

/**
 * @method getCCDisplay
 * @return {cc.Node}
 */
getCCDisplay : function (
)
{
    return cc.Node;
},

/**
 * @method DBCCArmature
 * @constructor
 * @param {dragonBones::ArmatureData} arg0
 * @param {dragonBones::Animation} arg1
 * @param {dragonBones::IEventDispatcher} arg2
 * @param {cc.Node} arg3
 */
DBCCArmature : function (
armaturedata, 
animation, 
ieventdispatcher, 
node 
)
{
},

};

/**
 * @class DBCCArmatureNode
 */
db.CCArmatureNode = {

/**
 * @method getAnimation
 * @return {dragonBones::Animation}
 */
getAnimation : function (
)
{
    return dragonBones::Animation;
},

/**
 * @method getCCEventDispatcher
 * @return {cc.EventDispatcher}
 */
getCCEventDispatcher : function (
)
{
    return cc.EventDispatcher;
},

/**
 * @method getArmature
 * @return {dragonBones::DBCCArmature}
 */
getArmature : function (
)
{
    return dragonBones::DBCCArmature;
},

/**
 * @method getCCSlot
 * @param {String} arg0
 * @return {dragonBones::DBCCSlot}
 */
getCCSlot : function (
str 
)
{
    return dragonBones::DBCCSlot;
},

/**
 * @method advanceTime
 * @param {float} arg0
 */
advanceTime : function (
float 
)
{
},

/**
 * @method initWithDBCCArmature
 * @param {dragonBones::DBCCArmature} arg0
 * @param {dragonBones::WorldClock} arg1
 * @return {bool}
 */
initWithDBCCArmature : function (
dbccarmature, 
worldclock 
)
{
    return false;
},

/**
 * @method getCCDisplay
 * @return {cc.Node}
 */
getCCDisplay : function (
)
{
    return cc.Node;
},

/**
 * @method create
 * @param {dragonBones::DBCCArmature} arg0
 * @return {dragonBones::DBCCArmatureNode}
 */
create : function (
dbccarmature 
)
{
    return dragonBones::DBCCArmatureNode;
},

/**
 * @method createWithWorldClock
 * @param {dragonBones::DBCCArmature} arg0
 * @param {dragonBones::WorldClock} arg1
 * @return {dragonBones::DBCCArmatureNode}
 */
createWithWorldClock : function (
dbccarmature, 
worldclock 
)
{
    return dragonBones::DBCCArmatureNode;
},

/**
 * @method DBCCArmatureNode
 * @constructor
 */
DBCCArmatureNode : function (
)
{
},

};

/**
 * @class DBCCFactory
 */
db.CCFactory = {

/**
 * @method buildArmatureNode
* @param {String|String|String} str
* @param {String|String} str
* @param {String} str
* @param {String} str
* @param {String} str
* @return {dragonBones::DBCCArmatureNode|dragonBones::DBCCArmatureNode|dragonBones::DBCCArmatureNode}
*/
buildArmatureNode : function(
str,
str,
str,
str,
str 
)
{
    return dragonBones::DBCCArmatureNode;
},

/**
 * @method hasDragonBones
 * @param {String} arg0
 * @param {String} arg1
 * @param {String} arg2
 * @return {bool}
 */
hasDragonBones : function (
str, 
str, 
str 
)
{
    return false;
},

/**
 * @method buildArmature
* @param {String|String|String} str
* @param {String|String} str
* @param {String} str
* @param {String} str
* @param {String} str
* @return {dragonBones::DBCCArmature|dragonBones::DBCCArmature|dragonBones::DBCCArmature}
*/
buildArmature : function(
str,
str,
str,
str,
str 
)
{
    return dragonBones::DBCCArmature;
},

/**
 * @method loadTextureAtlas
 * @param {String} arg0
 * @param {String} arg1
 * @return {dragonBones::ITextureAtlas}
 */
loadTextureAtlas : function (
str, 
str 
)
{
    return dragonBones::ITextureAtlas;
},

/**
 * @method refreshAllTextureAtlasTexture
 */
refreshAllTextureAtlasTexture : function (
)
{
},

/**
 * @method refreshTextureAtlasTexture
 * @param {String} arg0
 */
refreshTextureAtlasTexture : function (
str 
)
{
},

/**
 * @method loadDragonBonesData
 * @param {String} arg0
 * @param {String} arg1
 * @return {dragonBones::DragonBonesData}
 */
loadDragonBonesData : function (
str, 
str 
)
{
    return dragonBones::DragonBonesData;
},

/**
 * @method destroyInstance
 */
destroyInstance : function (
)
{
},

/**
 * @method getInstance
 * @return {dragonBones::DBCCFactory}
 */
getInstance : function (
)
{
    return dragonBones::DBCCFactory;
},

/**
 * @method DBCCFactory
 * @constructor
 */
DBCCFactory : function (
)
{
},

};
