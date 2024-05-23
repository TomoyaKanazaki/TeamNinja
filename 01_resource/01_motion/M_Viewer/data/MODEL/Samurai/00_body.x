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
 18;
 5.22658;3.74812;2.95718;,
 5.12400;3.74486;-1.68375;,
 4.19504;8.42542;-1.33775;,
 4.47288;8.51905;1.75805;,
 0.00000;3.94614;4.01982;,
 0.00000;8.94760;2.94086;,
 0.00000;3.31862;-4.13462;,
 0.00000;8.02861;-3.63007;,
 -5.22658;3.74812;2.95718;,
 -4.47288;8.51905;1.75804;,
 -4.19504;8.42542;-1.33775;,
 -5.12400;3.74486;-1.68375;,
 8.60119;-1.54674;-2.84580;,
 0.00000;-2.20760;-6.63354;,
 8.55200;-1.64512;5.74809;,
 -0.00000;-1.61697;7.17464;,
 -8.55200;-1.64512;5.74809;,
 -8.60119;-1.54674;-2.84580;;
 
 12;
 4;0,1,2,3;,
 4;4,0,3,5;,
 4;1,6,7,2;,
 4;8,9,10,11;,
 4;4,5,9,8;,
 4;11,10,7,6;,
 4;12,13,6,1;,
 4;14,12,1,0;,
 4;15,14,0,4;,
 4;15,4,8,16;,
 4;16,8,11,17;,
 4;17,11,6,13;;
 
 MeshMaterialList {
  2;
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
  0;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "C:\\Users\\student\\Desktop\\TeamNinja\\01_resource\\00_model\\Enemy\\Samurai\\source\\samurai_Tex_New.png";
   }
  }
  Material {
   0.416000;0.320000;0.064000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  24;
  0.982741;0.175717;-0.057825;,
  0.841314;0.540532;-0.003952;,
  0.820687;0.173966;-0.544251;,
  0.687735;0.537648;-0.487806;,
  -0.982741;0.175717;-0.057826;,
  -0.167019;0.503611;0.847633;,
  -0.820687;0.173966;-0.544251;,
  -0.687735;0.537648;-0.487806;,
  0.000000;0.510785;0.859708;,
  0.000000;0.141074;-0.989999;,
  0.000000;0.424144;-0.905595;,
  -0.000000;0.242950;0.970039;,
  0.000000;0.288677;-0.957426;,
  0.768974;0.363095;-0.526156;,
  0.930349;0.365319;-0.031509;,
  -0.000000;0.381854;0.924223;,
  -0.930349;0.365319;-0.031509;,
  -0.768975;0.363095;-0.526156;,
  0.203354;0.373875;0.904911;,
  0.235330;0.236126;0.942796;,
  -0.235331;0.236127;0.942795;,
  -0.203355;0.373875;0.904911;,
  0.167019;0.503611;0.847633;,
  -0.841314;0.540532;-0.003952;;
  12;
  4;14,13,2,0;,
  4;15,18,19,11;,
  4;13,12,9,2;,
  4;16,4,6,17;,
  4;15,11,20,21;,
  4;17,6,9,12;,
  4;3,10,12,13;,
  4;1,3,13,14;,
  4;8,22,18,15;,
  4;8,15,21,5;,
  4;23,16,17,7;,
  4;7,17,12,10;;
 }
 MeshTextureCoords {
  18;
  0.229880;0.377800;,
  0.151960;0.293490;,
  0.234150;0.237630;,
  0.272510;0.281410;,
  0.362920;0.381050;,
  0.362920;0.272790;,
  0.113600;0.160650;,
  0.212860;0.150660;,
  0.229880;0.377800;,
  0.272510;0.281410;,
  0.234150;0.237630;,
  0.151960;0.293490;,
  0.058110;0.357280;,
  0.000260;0.172040;,
  0.181200;0.487880;,
  0.362920;0.504690;,
  0.181200;0.487880;,
  0.058110;0.357280;;
 }
}
