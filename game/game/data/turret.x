xof 0303txt 0032

// DirectX - from CharacterFX

Frame turretFrame
{
    FrameTransformMatrix
    {
        1.000000,0.000000,0.000000,0.000000,
        0.000000,1.000000,0.000000,0.000000,
        0.000000,0.000000,1.000000,0.000000,
        0.000000,0.000000,0.000000,1.000000;;
    }

    Mesh turretMesh
    {
        8;
        0.551558;0.001408;-0.677550;,
        -0.576234;0.002261;0.456548;,
        0.551892;0.003376;0.456214;,
        -0.576568;0.000293;-0.677216;,
        0.326097;0.202349;-0.450663;,
        -0.343449;0.200035;0.221898;,
        -0.341751;0.199882;-0.450963;,
        0.326163;0.203515;0.221501;;
        12;
        3;0,2,1;,
        3;0,1,3;,
        3;4,6,5;,
        3;4,5,7;,
        3;0,3,6;,
        3;0,6,4;,
        3;3,1,5;,
        3;3,5,6;,
        3;1,2,7;,
        3;1,7,5;,
        3;4,7,2;,
        3;4,2,0;;

        MeshNormals
        {
            8;
            0.117383;-0.976047;-0.183178;,
            -0.115632;-0.975257;0.188420;,
            0.487126;-0.817756;0.306566;,
            -0.485472;-0.819502;-0.304521;,
            0.236088;0.967508;-0.090504;,
            -0.236830;0.967821;0.085054;,
            -0.125377;0.934964;-0.331847;,
            0.119854;0.937537;0.326587;;
            12;
            3;0,2,1;,
            3;0,1,3;,
            3;4,6,5;,
            3;4,5,7;,
            3;0,3,6;,
            3;0,6,4;,
            3;3,1,5;,
            3;3,5,6;,
            3;1,2,7;,
            3;1,7,5;,
            3;4,7,2;,
            3;4,2,0;;
        }

        MeshTextureCoords
        {
            8;
            1.099520;1.099520;,
            -0.099520;-0.099520;,
            -0.099519;1.099521;,
            1.099520;-0.099521;,
            -0.000761;0.750048;,
            0.663396;0.748268;,
            0.000000;-1.000000;,
            -0.645503;0.763758;;
        }

        MeshMaterialList
        {
            1;
            12;
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0;

            Material Material0
            {
                0.800000;0.800000;0.800000;1.000000;;
                0.000000;
                0.000000;0.000000;0.000000;;
                0.000000;0.000000;0.000000;;

                TextureFileName
                {
                    "turret.bmp";
                }
            }
        }
    }
}
