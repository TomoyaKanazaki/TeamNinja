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
 12;
 2.64550;2.10698;-1.11239;,
 1.29785;8.41206;0.67823;,
 -1.07175;8.06144;-1.58450;,
 1.29107;2.09526;-2.99124;,
 0.30627;1.46308;-0.19222;,
 -1.85658;7.20286;2.21181;,
 4.89654;1.46349;-1.83506;,
 -2.53796;13.37241;-2.77928;,
 2.20124;14.07364;1.74618;,
 2.18768;1.44005;-5.59278;,
 -4.10762;11.65525;4.81334;,
 0.21808;0.17569;0.00528;;
 
 12;
 3;0,1,2;,
 3;0,2,3;,
 3;0,4,5;,
 3;0,5,1;,
 3;4,3,2;,
 3;4,2,5;,
 3;6,7,8;,
 3;6,9,7;,
 3;6,10,11;,
 3;6,8,10;,
 3;11,7,9;,
 3;11,10,7;;
 
 MeshMaterialList {
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
  0;;
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  18;
  -0.684535;-0.377916;0.623371;,
  -0.624638;-0.333981;0.705892;,
  -0.735992;-0.417192;0.533166;,
  -0.408539;0.217348;-0.886485;,
  -0.443272;0.181067;-0.877909;,
  -0.476855;0.144317;-0.867054;,
  0.914763;0.256840;0.311837;,
  0.889497;0.264401;0.372675;,
  0.936006;0.248151;0.249627;,
  0.684535;0.377917;-0.623370;,
  0.624638;0.333981;-0.705892;,
  0.735993;0.417193;-0.533164;,
  0.408539;-0.217348;0.886485;,
  0.443272;-0.181067;0.877909;,
  0.476855;-0.144317;0.867054;,
  -0.914763;-0.256840;-0.311836;,
  -0.889498;-0.264401;-0.372674;,
  -0.936006;-0.248150;-0.249627;;
  12;
  3;0,1,0;,
  3;0,0,2;,
  3;4,3,4;,
  3;4,4,5;,
  3;6,7,6;,
  3;6,6,8;,
  3;9,9,10;,
  3;9,11,9;,
  3;13,13,12;,
  3;13,14,13;,
  3;15,15,16;,
  3;15,17,15;;
 }
 MeshTextureCoords {
  12;
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
