xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 132;
 -3.26940;-1.39485;3.31065;,
 -5.22396;-1.38441;3.12816;,
 -5.32656;-2.44842;2.00799;,
 -3.30432;-2.53281;1.68390;,
 -3.28344;-3.30144;0.27072;,
 -5.49522;-3.30831;0.51354;,
 -5.61663;-3.26289;-0.65697;,
 -3.25968;-3.22128;-1.16082;,
 -7.54371;3.30576;0.47613;,
 -7.32237;2.09058;2.47635;,
 -6.51735;1.85115;2.55096;,
 -6.66912;3.20916;0.64908;,
 -3.21009;-0.04086;3.79206;,
 -5.15646;-0.08379;3.64053;,
 -7.41852;-3.38100;-0.09081;,
 -7.59459;-2.48601;-1.82310;,
 -5.66826;-2.29473;-1.95966;,
 -3.13389;3.58302;-0.61644;,
 -5.55354;3.31650;-0.68550;,
 -5.22222;3.25143;0.76977;,
 -3.13008;3.20067;1.15251;,
 -3.17493;1.57464;3.11604;,
 -5.12283;1.65933;2.84832;,
 -7.76100;2.33292;-2.29023;,
 -7.94214;3.13728;-1.05579;,
 -5.62671;2.51820;-2.00571;,
 -3.20337;-2.20767;-2.46972;,
 -5.77098;-0.24363;-2.84040;,
 -3.18663;-0.05778;-3.43713;,
 -5.74122;1.39473;-2.71917;,
 -3.15459;1.74117;-3.16953;,
 -3.13281;3.01731;-2.06763;,
 -7.28433;-1.03266;3.30654;,
 -7.18164;-2.38104;2.44311;,
 -7.29945;-3.25074;1.06914;,
 -7.55904;0.23439;3.49155;,
 -7.73658;-0.40998;-2.89854;,
 -7.79055;1.09350;-2.92137;,
 -8.28306;1.98666;2.46300;,
 -8.43261;3.19770;0.40716;,
 -6.97149;-1.02303;1.21071;,
 -6.89022;0.14076;0.28395;,
 -7.06062;-1.52280;0.68058;,
 -6.97623;-0.44598;1.79523;,
 -6.89022;0.14076;0.28395;,
 -6.97149;-1.02303;1.21071;,
 -7.00719;0.31803;1.99986;,
 -6.89022;0.14076;0.28395;,
 -6.97623;-0.44598;1.79523;,
 -7.35396;1.31931;1.48011;,
 -6.89022;0.14076;0.28395;,
 -7.00719;0.31803;1.99986;,
 -7.42758;1.78149;0.29367;,
 -6.89022;0.14076;0.28395;,
 -7.35396;1.31931;1.48011;,
 -7.28448;1.67994;-0.42477;,
 -6.89022;0.14076;0.28395;,
 -7.42758;1.78149;0.29367;,
 -7.28130;1.23687;-1.00314;,
 -6.89022;0.14076;0.28395;,
 -7.28448;1.67994;-0.42477;,
 -7.29609;0.61713;-1.31868;,
 -6.89022;0.14076;0.28395;,
 -7.28130;1.23687;-1.00314;,
 -7.26906;-0.13464;-1.30734;,
 -6.89022;0.14076;0.28395;,
 -7.29609;0.61713;-1.31868;,
 -7.23231;-1.03056;-0.65796;,
 -6.89022;0.14076;0.28395;,
 -7.26906;-0.13464;-1.30734;,
 -7.11006;-1.62009;0.09657;,
 -7.06062;-1.52280;0.68058;,
 -6.89022;0.14076;0.28395;,
 -7.11006;-1.62009;0.09657;,
 -6.89022;0.14076;0.28395;,
 -7.23231;-1.03056;-0.65796;,
 -7.29945;-3.25074;1.06914;,
 -7.18164;-2.38104;2.44311;,
 -7.28433;-1.03266;3.30654;,
 -7.55904;0.23439;3.49155;,
 -8.28306;1.98666;2.46300;,
 -8.43261;3.19770;0.40716;,
 -7.94214;3.13728;-1.05579;,
 -7.76100;2.33292;-2.29023;,
 -7.79055;1.09350;-2.92137;,
 -7.79055;1.09350;-2.92137;,
 -7.73658;-0.40998;-2.89854;,
 -7.59459;-2.48601;-1.82310;,
 -7.41852;-3.38100;-0.09081;,
 -1.57179;3.76092;-0.59445;,
 -1.58076;3.17499;-2.25684;,
 -1.57359;1.76100;-3.63207;,
 0.98916;1.63926;-3.60309;,
 1.01454;-0.07659;-4.21260;,
 -1.40328;-0.08820;-4.01043;,
 0.92073;3.03984;-1.94415;,
 -1.14318;-2.35974;-3.31299;,
 -1.00017;-3.22539;-1.80318;,
 2.26035;1.48218;-3.15771;,
 3.48822;-0.26490;-2.57259;,
 3.06234;-1.19466;-3.17298;,
 2.10177;0.04257;-4.22118;,
 2.69346;2.55312;-1.40427;,
 3.82992;0.77067;-1.30272;,
 1.21158;-2.20695;-3.62148;,
 -1.19895;-1.78110;3.32649;,
 -1.08186;-2.93607;1.78302;,
 -1.02018;-3.52692;0.03162;,
 -1.31850;-0.25551;4.08609;,
 -1.56603;3.28428;1.63146;,
 1.42956;-3.00876;-2.06856;,
 3.30567;-2.24670;-1.60221;,
 1.67700;-3.37800;-0.15660;,
 3.50385;-2.44296;-0.19140;,
 -1.42413;1.50552;3.52209;,
 3.84999;-1.20819;-2.13357;,
 4.12581;-0.98829;-1.07805;,
 1.80834;0.04257;4.41828;,
 3.06234;-1.19466;3.37014;,
 3.48822;-0.26490;2.76972;,
 2.26035;1.48218;3.35481;,
 0.72720;1.63926;3.65244;,
 0.72108;-0.07659;4.40967;,
 2.69346;2.55312;1.60137;,
 0.86877;3.03984;1.82322;,
 3.82992;0.77067;1.13682;,
 1.21158;-2.20695;3.81855;,
 3.30567;-2.24670;1.79937;,
 1.42956;-3.00876;2.26572;,
 3.84999;-1.20819;2.33070;,
 4.12581;-0.98829;0.54396;,
 1.16061;3.52935;-0.64716;;
 
 108;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;12,13,1,0;,
 4;14,15,16,6;,
 4;17,18,19,20;,
 4;3,2,5,4;,
 4;21,22,13,12;,
 4;23,24,18,25;,
 4;26,16,27,28;,
 4;28,27,29,30;,
 4;30,29,25,31;,
 4;1,32,33,2;,
 4;5,34,14,6;,
 4;13,35,32,1;,
 4;18,24,11,19;,
 4;2,33,34,5;,
 4;22,10,35,13;,
 4;16,15,36,27;,
 4;27,36,37,29;,
 4;29,37,23,25;,
 3;9,38,35;,
 3;11,24,8;,
 4;11,10,22,19;,
 3;39,8,24;,
 3;35,10,9;,
 4;39,38,9,8;,
 4;19,22,21,20;,
 4;6,16,26,7;,
 4;25,18,17,31;,
 3;40,41,42;,
 3;43,44,45;,
 3;46,47,48;,
 3;49,50,51;,
 3;52,53,54;,
 3;55,56,57;,
 3;58,59,60;,
 3;61,62,63;,
 3;64,65,66;,
 3;67,68,69;,
 3;70,71,72;,
 3;73,74,75;,
 4;76,77,40,42;,
 4;77,78,43,45;,
 4;46,48,78,79;,
 4;49,51,79,80;,
 4;80,81,52,54;,
 4;81,82,55,57;,
 4;58,60,82,83;,
 4;83,84,61,63;,
 4;85,86,64,66;,
 4;86,87,67,69;,
 4;87,88,73,75;,
 4;76,71,70,88;,
 4;31,17,89,90;,
 4;91,92,93,94;,
 4;91,90,95,92;,
 4;7,26,96,97;,
 4;98,99,100,101;,
 4;98,101,93,92;,
 4;98,92,95,102;,
 4;98,102,103,99;,
 4;96,94,93,104;,
 4;105,0,3,106;,
 4;107,4,7,97;,
 4;108,12,0,105;,
 4;89,17,20,109;,
 4;106,3,4,107;,
 4;96,26,28,94;,
 4;94,28,30,91;,
 4;91,30,31,90;,
 3;104,101,100;,
 4;110,97,96,104;,
 4;110,104,100,111;,
 4;112,107,97,110;,
 4;110,111,113,112;,
 3;104,93,101;,
 4;109,20,21,114;,
 4;12,108,114,21;,
 3;115,100,99;,
 3;115,111,100;,
 4;115,99,103,116;,
 4;113,111,115,116;,
 4;117,118,119,120;,
 4;121,122,117,120;,
 4;123,124,121,120;,
 4;119,125,123,120;,
 3;118,117,126;,
 4;127,118,126,128;,
 4;112,113,127,128;,
 3;117,122,126;,
 3;119,118,129;,
 3;118,127,129;,
 4;130,125,119,129;,
 4;130,129,127,113;,
 4;128,106,107,112;,
 4;106,128,126,105;,
 4;105,126,122,108;,
 4;121,114,108,122;,
 4;124,109,114,121;,
 3;113,116,130;,
 4;116,103,125,130;,
 4;123,125,103,102;,
 3;131,102,95;,
 3;131,124,123;,
 3;131,123,102;,
 4;89,131,95,90;,
 4;109,124,131,89;;
 
 MeshMaterialList {
  1;
  108;
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
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.316800;0.674400;0.721600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  108;
  -0.158774;-0.021899;0.987072;,
  -0.110403;0.478424;-0.871161;,
  -0.034360;-0.995556;0.087674;,
  -0.172529;-0.629925;0.757250;,
  0.542938;0.483755;-0.686439;,
  -0.169508;-0.623751;-0.763022;,
  -0.017392;0.997206;-0.072654;,
  0.832347;0.205867;-0.514604;,
  -0.035093;0.840615;-0.540494;,
  -0.195878;-0.054847;-0.979093;,
  -0.057996;-0.940435;-0.334991;,
  0.609617;0.728438;-0.312643;,
  -0.063345;0.893737;0.444097;,
  0.380461;-0.000579;-0.924797;,
  -0.117239;-0.893568;0.433348;,
  0.142007;0.570273;-0.809088;,
  0.675067;-0.322475;-0.663547;,
  0.918603;0.334821;-0.209911;,
  0.219047;0.832473;-0.508927;,
  0.013214;0.042881;-0.998993;,
  -0.070650;0.626612;0.776123;,
  -0.073530;-0.642257;-0.762954;,
  -0.010869;-0.523923;0.851696;,
  -0.065016;0.976122;-0.207265;,
  0.048178;-0.966076;0.253723;,
  -0.075022;0.320266;-0.944352;,
  -0.025999;-0.953878;-0.299067;,
  -0.069124;0.731691;-0.678122;,
  -0.062104;0.929625;0.363237;,
  0.056667;-0.794745;0.604292;,
  -0.079041;-0.178070;-0.980838;,
  -0.071316;0.065507;0.995300;,
  -0.009310;0.684495;0.728958;,
  -0.000826;-0.665670;-0.746246;,
  0.012574;-0.463608;0.885951;,
  -0.006421;0.982603;-0.185607;,
  0.069426;-0.957867;0.278694;,
  0.053175;0.268503;-0.961810;,
  0.083409;-0.758777;0.645988;,
  0.041538;-0.204508;-0.977983;,
  -0.026382;0.303656;0.952416;,
  0.002076;-0.966233;-0.257663;,
  0.026889;0.690564;-0.722772;,
  -0.002191;0.974661;0.223674;,
  0.033215;0.961172;0.273944;,
  -0.123453;0.965270;0.230247;,
  -0.095844;0.688906;0.718486;,
  0.010578;0.661790;0.749615;,
  -0.102464;-0.935874;-0.337107;,
  -0.160060;0.792776;-0.588121;,
  -0.111727;0.900159;0.420988;,
  -0.129182;0.581638;0.803125;,
  -0.216516;-0.613118;-0.759741;,
  -0.095039;-0.600814;0.793719;,
  -0.113092;0.987266;-0.111874;,
  -0.037775;-0.981402;0.188210;,
  -0.225036;0.396893;-0.889851;,
  -0.056252;-0.848144;0.526771;,
  -0.261808;-0.123675;-0.957163;,
  -0.124137;0.001686;0.992264;,
  0.250597;-0.901007;-0.354101;,
  0.622939;-0.699157;-0.350894;,
  0.178750;-0.554679;-0.812637;,
  0.240952;-0.970536;0.001528;,
  0.747012;-0.664480;-0.020980;,
  -0.128127;0.565315;0.814864;,
  0.981251;-0.147013;-0.124632;,
  0.873180;-0.240783;-0.423770;,
  -0.995179;-0.069088;0.069610;,
  -0.989637;0.134769;0.049558;,
  -0.995481;0.090545;-0.028629;,
  -0.991504;0.000959;-0.130075;,
  -0.951824;-0.240779;-0.189885;,
  -0.880029;-0.441155;-0.175870;,
  -0.889199;-0.429817;-0.156785;,
  -0.944127;-0.329334;-0.012751;,
  -0.966616;-0.171874;0.190036;,
  -0.965615;-0.065315;0.251637;,
  -0.969865;0.034180;0.241232;,
  -0.970945;0.101400;0.216756;,
  -0.982619;0.120657;0.141077;,
  0.498466;0.508361;0.702212;,
  0.838523;0.202707;0.505755;,
  0.587208;0.763552;0.268655;,
  0.333913;0.021983;0.942348;,
  0.041044;0.615468;0.787093;,
  0.657002;-0.311803;0.686387;,
  0.928037;0.328104;0.176337;,
  0.145755;0.893033;0.425732;,
  -0.053566;0.048365;0.997392;,
  0.191134;-0.909230;0.369823;,
  0.633837;-0.700554;0.327833;,
  0.114057;-0.559483;0.820956;,
  0.985609;-0.142033;0.091655;,
  0.888573;-0.237694;0.392351;,
  0.280708;0.957559;-0.065452;,
  -0.987445;0.153349;0.037901;,
  -0.992321;0.105408;-0.064714;,
  -0.978684;-0.004405;-0.205325;,
  -0.897807;-0.292351;-0.329352;,
  -0.784907;-0.541849;-0.300534;,
  -0.814524;-0.541824;-0.207310;,
  -0.913032;-0.407291;-0.022056;,
  -0.957667;-0.206024;0.201065;,
  -0.958887;-0.070557;0.274877;,
  -0.965838;0.027184;0.257718;,
  -0.972398;0.097974;0.211761;,
  -0.981914;0.134355;0.133393;;
  108;
  4;53,22,29,57;,
  4;55,24,26,48;,
  4;43,32,47,44;,
  4;59,31,22,53;,
  4;41,33,21,26;,
  4;54,23,28,50;,
  4;57,29,24,55;,
  4;51,20,31,59;,
  4;42,35,23,27;,
  4;52,21,30,58;,
  4;58,30,25,56;,
  4;56,25,27,49;,
  4;22,34,38,29;,
  4;24,36,41,26;,
  4;31,40,34,22;,
  4;23,35,44,28;,
  4;29,38,36,24;,
  4;20,47,40,31;,
  4;21,33,39,30;,
  4;30,39,37,25;,
  4;25,37,42,27;,
  3;32,46,40;,
  3;44,35,43;,
  4;44,47,20,28;,
  3;45,43,35;,
  3;40,47,32;,
  4;45,46,32,43;,
  4;28,20,51,50;,
  4;26,21,52,48;,
  4;27,23,54,49;,
  3;70,68,69;,
  3;71,68,70;,
  3;72,68,71;,
  3;73,68,72;,
  3;74,68,73;,
  3;75,68,74;,
  3;76,68,75;,
  3;77,68,76;,
  3;78,68,77;,
  3;79,68,78;,
  3;80,69,68;,
  3;80,68,79;,
  4;96,97,70,69;,
  4;97,98,71,70;,
  4;72,71,98,99;,
  4;73,72,99,100;,
  4;100,101,74,73;,
  4;101,102,75,74;,
  4;76,75,102,103;,
  4;103,104,77,76;,
  4;104,105,78,77;,
  4;105,106,79,78;,
  4;106,107,80,79;,
  4;96,69,80,107;,
  4;49,54,6,8;,
  4;1,15,19,9;,
  4;1,8,18,15;,
  4;48,52,5,10;,
  4;4,7,16,13;,
  4;4,13,19,15;,
  4;4,15,18,11;,
  4;4,11,17,7;,
  4;5,9,19,62;,
  4;3,53,57,14;,
  4;2,55,48,10;,
  4;0,59,53,3;,
  4;6,54,50,12;,
  4;14,57,55,2;,
  4;5,52,58,9;,
  4;9,58,56,1;,
  4;1,56,49,8;,
  3;62,13,16;,
  4;60,10,5,62;,
  4;60,62,16,61;,
  4;63,2,10,60;,
  4;60,61,64,63;,
  3;62,19,13;,
  4;12,50,51,65;,
  4;59,0,65,51;,
  3;67,16,7;,
  3;67,61,16;,
  4;67,7,17,66;,
  4;64,61,67,66;,
  4;84,86,82,81;,
  4;85,89,84,81;,
  4;83,88,85,81;,
  4;82,87,83,81;,
  3;86,84,92;,
  4;91,86,92,90;,
  4;63,64,91,90;,
  3;84,89,92;,
  3;82,86,94;,
  3;86,91,94;,
  4;93,87,82,94;,
  4;93,94,91,64;,
  4;90,14,2,63;,
  4;14,90,92,3;,
  4;3,92,89,0;,
  4;85,65,0,89;,
  4;88,12,65,85;,
  3;64,66,93;,
  4;66,17,87,93;,
  4;83,87,17,11;,
  3;95,11,18;,
  3;95,88,83;,
  3;95,83,11;,
  4;6,95,18,8;,
  4;12,88,95,6;;
 }
 MeshTextureCoords {
  132;
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.395830;0.250000;,
  0.375000;0.000000;,
  0.354170;0.250000;,
  0.479170;0.250000;,
  0.458330;0.000000;,
  0.437500;0.250000;,
  0.562500;0.250000;,
  0.541670;0.000000;,
  0.520830;0.250000;,
  0.645830;0.250000;,
  0.625000;0.000000;,
  0.604170;0.250000;,
  0.729170;0.250000;,
  0.708330;0.000000;,
  0.687500;0.250000;,
  0.812500;0.250000;,
  0.791670;0.000000;,
  0.770830;0.250000;,
  0.895830;0.250000;,
  0.875000;0.000000;,
  0.854170;0.250000;,
  0.979170;0.250000;,
  0.958330;0.000000;,
  0.937500;0.250000;,
  0.062500;0.250000;,
  0.041670;0.000000;,
  0.020830;0.250000;,
  0.145830;0.250000;,
  0.125000;0.000000;,
  0.104170;0.250000;,
  0.270830;0.250000;,
  0.312500;0.250000;,
  0.291670;0.000000;,
  0.229170;0.250000;,
  0.208330;0.000000;,
  0.187500;0.250000;,
  0.333330;0.500000;,
  0.416670;0.500000;,
  0.500000;0.500000;,
  0.583330;0.500000;,
  0.666670;0.500000;,
  0.750000;0.500000;,
  0.833330;0.500000;,
  0.916670;0.500000;,
  1.000000;0.500000;,
  0.000000;0.500000;,
  0.083330;0.500000;,
  0.166670;0.500000;,
  0.250000;0.500000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;;
 }
}