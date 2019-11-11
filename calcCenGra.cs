using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class calcCenGra : MonoBehaviour
{
    GameObject gopm;
    GameObject[] go = new GameObject[26];

    float[] masslist = {
        81.0f, 0, 0, 0, 0,
        90.86f, 90.86f, 0, 0, 89.29f,
        89.29f, 0, 0, 0, 0,
        73.67f, 73.67f, 115.77f, 115.77f, 47.48f,
        47.48f, 72.60f, 72.60f, 107.48f, 107.48f,
        550.0f };

    // Start is called before the first frame update
    void Start()
    {
        gopm = GameObject.Find("PremaidAI");
        if (gopm == null)
        {
            Debug.Log("hoge");
        }
        /*
        go[0] = GameObject.Find("Cube0");
        go[1] = GameObject.Find("Cube1");
        go[2] = GameObject.Find("Cube2");
        go[3] = GameObject.Find("Cube3");
        go[4] = GameObject.Find("Cube4");
        go[5] = GameObject.Find("Cube5");
        go[6] = GameObject.Find("Cube1");
        go[7] = GameObject.Find("Cube2");
        go[8] = GameObject.Find("Cube3");
        go[9] = GameObject.Find("Cube4");
        go[10] = GameObject.Find("Cube5");
        go[11] = GameObject.Find("Cube0");
        go[12] = GameObject.Find("Cube1");
        go[13] = GameObject.Find("Cube2");
        go[14] = GameObject.Find("Cube3");
        go[15] = GameObject.Find("Cube4");
        go[16] = GameObject.Find("Cube5");
        go[17] = GameObject.Find("Cube0");
        go[18] = GameObject.Find("Cube1");
        go[19] = GameObject.Find("Cube2");
        go[20] = GameObject.Find("Cube3");
        go[21] = GameObject.Find("Cube4");
        go[22] = GameObject.Find("Cube5");
        go[23] = GameObject.Find("Cube4");
        go[24] = GameObject.Find("Cube5");
        go[25] = GameObject.Find("Cube0");
        */

        go[0] = GameObject.Find("Head");
        if (go[0] == null)
        {
            Debug.Log("hogehoge");
        }
        go[1] = GameObject.Find("Neck_Yaw");
        go[2] = GameObject.Find("Neck_Pitch");
        go[3] = GameObject.Find("Shoulder.L");
        go[4] = GameObject.Find("Shoulder.R");
        //
        go[5] = GameObject.Find("UpperArm.L");
        go[6] = GameObject.Find("UpperArm.R");
        go[7] = GameObject.Find("Elbow.L");
        go[8] = GameObject.Find("Elbow.R");
        go[9] = GameObject.Find("LowerArm.L");

        go[10] = GameObject.Find("LowerArm.R");
        go[11] = GameObject.Find("Hand.L");
        go[12] = GameObject.Find("Hand.R");
        go[13] = GameObject.Find("HipJoint.L");
        go[14] = GameObject.Find("HipJoint.R");

        go[15] = GameObject.Find("LegRoot.L");
        go[16] = GameObject.Find("LegRoot.R");
        go[17] = GameObject.Find("UpperLeg.L");
        go[18] = GameObject.Find("UpperLeg.R");
        go[19] = GameObject.Find("LowerLeg.L");

        go[20] = GameObject.Find("LowerLeg.R");
        go[21] = GameObject.Find("Ancle.L");
        go[22] = GameObject.Find("Ancle.R");
        go[23] = GameObject.Find("Foot.L_end");
        go[24] = GameObject.Find("Foot.R_end");

        go[25] = GameObject.Find("Spine");
    }/// <summary>
    /// /
    /// </summary>

    // Update is called once per frame
    void Update()
    {
        float sx,sy,sz;
        float allmass;

        sx = 0.0f;
        sy = 0.0f;
        sz = 0.0f;
        allmass = 0.0f;

        for ( int a=0; a<=25; a++)
        {
            sx += go[a].transform.position.x * masslist[a];
            sy += go[a].transform.position.y * masslist[a];
            sz += go[a].transform.position.z * masslist[a];
            allmass += masslist[a];
        }

        this.transform.SetPositionAndRotation(new Vector3((sx / allmass), (sy / allmass), (sz / allmass)), this.transform.rotation);
    }
}
