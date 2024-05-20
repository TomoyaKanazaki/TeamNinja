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
 -0.31553;10.17543;6.27330;,
 -5.33703;-0.04957;6.31692;,
 -1.38656;-0.04957;-9.37950;,
 4.21914;4.90043;-6.45774;,
 3.59638;-0.04957;9.38636;,
 9.84743;-0.04957;-3.98874;,
 8.18734;-0.04957;-10.74834;,
 4.69557;-0.04957;-12.23050;,
 -1.78226;-0.04957;10.80762;,
 -0.31553;10.17543;6.27330;,
 -1.38656;-0.04957;-9.37950;,
 -5.33703;-0.04957;6.31692;,
 4.21914;4.90043;-6.45774;,
 3.59638;-0.04957;9.38636;,
 9.84743;-0.04957;-3.98874;,
 8.18734;-0.04957;-10.74834;,
 4.69557;-0.04957;-12.23050;,
 -1.78226;-0.04957;10.80762;;
 
 18;
 3;0,1,2;,
 3;0,2,3;,
 3;4,0,3;,
 3;4,3,5;,
 3;5,3,6;,
 3;6,3,7;,
 3;3,2,7;,
 3;4,8,0;,
 3;0,8,1;,
 3;9,10,11;,
 3;9,12,10;,
 3;13,12,9;,
 3;13,14,12;,
 3;14,15,12;,
 3;15,16,12;,
 3;12,16,10;,
 3;13,9,17;,
 3;9,11,17;;
 
 MeshMaterialList {
  1;
  18;
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
   0.121569;0.188235;0.349020;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  24;
  0.793658;-0.606744;0.044373;,
  0.879832;-0.432922;-0.196147;,
  0.568133;-0.675624;0.469847;,
  0.164319;-0.779679;0.604235;,
  -0.644509;-0.579410;-0.498890;,
  -0.647359;-0.760597;-0.049180;,
  -0.501374;-0.760151;0.413274;,
  0.019000;-0.758219;0.651723;,
  -0.237844;-0.365035;-0.900100;,
  -0.905014;-0.394607;-0.158855;,
  -0.752944;-0.652101;-0.088535;,
  0.731055;-0.361490;-0.578692;,
  -0.793658;0.606744;-0.044373;,
  -0.879832;0.432922;0.196147;,
  -0.568133;0.675624;-0.469847;,
  -0.164319;0.779679;-0.604235;,
  0.644509;0.579410;0.498890;,
  0.647359;0.760597;0.049180;,
  0.501374;0.760151;-0.413274;,
  -0.019000;0.758219;-0.651723;,
  0.237844;0.365035;0.900100;,
  0.752944;0.652101;0.088535;,
  0.905014;0.394607;0.158855;,
  -0.731055;0.361490;0.578692;;
  18;
  3;0,1,2;,
  3;0,2,3;,
  3;4,9,10;,
  3;4,10,5;,
  3;5,10,6;,
  3;6,3,7;,
  3;3,2,7;,
  3;4,8,8;,
  3;0,11,1;,
  3;12,14,13;,
  3;12,15,14;,
  3;16,21,22;,
  3;16,17,21;,
  3;17,18,21;,
  3;18,19,15;,
  3;15,19,14;,
  3;16,20,20;,
  3;12,13,23;;
 }
 MeshTextureCoords {
  18;
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
