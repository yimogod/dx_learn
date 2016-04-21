#pragma once

class EulerAngles{
public:
	/* rotate y, base on self-coordinate, 180~-180 */
	float heading;
	/* rotate x, 90~-90 */
	float pitch;
	/* roatet z, 180~-180 */
	float bank;
public:
	EulerAngles();
	EulerAngles(float h, float p, float b) : heading(h), pitch(p), bank(b){}
    ~EulerAngles();
    
	/*置0*/
	void identity(){ heading = pitch = bank = 0; }
	/*限制欧拉角到正确范围*/
	void restrict();
	/*镜头在direction(heading方向)前进or后退, d为增量*/

};
