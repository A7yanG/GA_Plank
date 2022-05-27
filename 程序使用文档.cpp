
/*
 算法使用流程：
 1.调用PlacePlanksInitialization函数，实现程序初始化
 2.调用PlankPlaceEncodeByArea函数，进行木板放置
 3.调用GetPlacedPlanksFromBestSolution函数，获得摆放好的某一层木板数量和各个木板的信息
 */

/**
 * @description: 程序初始化
 * @param {int} tableLength 小车长度
 * @param {int} tableWidth 小车宽度
 * @param {int} planks int型木板数组，按照每一块木板的长和宽依次存储
 * @param {int} plankNum 木板数量
 * @param {double} scale    缩放尺度，如1.1表示实际可摆放的面积为小车面积的1.1倍
 * @return {*}
 */
void PlacePlanksInitialization(int tableLength,int tableWidth,int planks[],int plankNum,double scale)

/**
 * @description: 根据木板面积开始摆放
 * @return {*}
 */
void PlankPlaceEncodeByArea()

/**
 * @description: 获得某一层木板数量和各个木板的信息
 * @param {int} index   层数下标（从0开始）
 * @param {int&} plankNum   木板数量
 * @param {int*} plankId    木板ID数组    
 * @param {int*} length     木板长度数组
 * @param {int*} width      木板宽度数组
 * @param {int*} x          木板左上角x坐标数组
 * @param {int*} y          木板左上角y坐标数组
 * @param {int*} rotate     木板是否旋转数组
 * @return {bool}           返回false则表示木板层数下标超过最大层数，否则返回true 
 */
bool GetPlacedPlanksFromBestSolution(int index,int& plankNum,int* plankId,int* length,int* width,int* x,int* y,int* rotate)

