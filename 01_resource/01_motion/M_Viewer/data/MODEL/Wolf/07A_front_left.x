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
 38;
 -0.01556;5.09052;2.79658;,
 0.82866;2.12414;1.57408;,
 0.95668;2.46148;-1.18112;,
 0.04466;5.85188;-0.25590;,
 -2.95712;-5.39642;6.03066;,
 0.20414;-5.05684;4.76978;,
 0.21760;-2.26418;4.62122;,
 -0.72944;-1.28330;6.11208;,
 0.21282;-10.11416;-1.11676;,
 -0.74688;-14.59818;0.12556;,
 -2.53416;-14.48342;-0.03472;,
 -2.21962;-10.17110;-1.23848;,
 0.02982;5.68846;-3.70218;,
 0.82236;2.77614;-3.12788;,
 0.73840;-0.13222;-4.68480;,
 -0.75672;1.40228;-6.14718;,
 1.60700;-2.53340;1.81270;,
 1.67626;-5.40584;2.20822;,
 1.55840;-1.84564;-1.34660;,
 1.73766;-5.58474;-0.77200;,
 0.70816;-5.39078;-3.24352;,
 0.38786;1.21212;3.54972;,
 -3.55898;-14.17122;1.92678;,
 -4.05186;-9.91340;0.11292;,
 -0.11538;3.51604;5.01060;,
 -1.23650;-3.86620;-5.42426;,
 -0.20702;-8.32526;5.23560;,
 -0.81300;-11.45690;5.40238;,
 0.20960;-12.38084;4.19234;,
 1.32888;-8.95382;3.70022;,
 0.15166;-14.27818;1.96486;,
 1.43154;-10.04992;0.73142;,
 -2.81172;-8.37386;6.14464;,
 -3.08592;-11.28424;5.53320;,
 -4.14162;-9.38578;3.16184;,
 -3.72634;-12.52674;3.72698;,
 -3.50870;-6.41932;-3.49706;,
 -3.71646;-6.70196;1.92288;;
 
 29;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;12,13,14,15;,
 4;16,6,5,17;,
 4;18,19,20,14;,
 4;16,1,21,6;,
 4;18,14,13,2;,
 4;11,10,22,23;,
 4;7,6,21,24;,
 4;3,2,13,12;,
 4;15,14,20,25;,
 4;26,27,28,29;,
 4;18,2,1,16;,
 4;18,16,17,19;,
 4;24,21,1,0;,
 4;29,28,30,31;,
 4;32,33,27,26;,
 4;34,35,33,32;,
 4;23,22,35,34;,
 4;31,30,9,8;,
 4;19,31,8,20;,
 4;36,23,34,37;,
 4;37,34,32,4;,
 4;4,32,26,5;,
 4;17,29,31,19;,
 4;5,26,29,17;,
 4;25,11,23,36;,
 4;20,8,11,25;;
 
 MeshMaterialList {
  8;
  29;
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1;;
  Material {
   0.008000;0.008000;0.008000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.373600;0.373600;0.323200;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.734400;0.765600;0.687200;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.016000;0.016000;0.016000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.400000;0.144000;0.104000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.776000;0.688000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.400000;0.400000;0.400000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.776000;0.688000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  42;
  0.889761;0.020482;-0.455968;,
  0.953516;0.196113;-0.228794;,
  0.716788;-0.263651;-0.645525;,
  0.840749;0.014138;0.541240;,
  0.936107;0.099346;0.337394;,
  0.670576;0.005551;0.741820;,
  0.992674;0.063153;-0.103003;,
  0.981369;0.192033;-0.006109;,
  0.977782;-0.084897;-0.191662;,
  0.969842;0.095385;0.224296;,
  0.956725;0.023374;0.290055;,
  0.970592;0.164533;0.175726;,
  0.547180;-0.376785;-0.747413;,
  0.268326;-0.021916;0.963079;,
  -0.987738;0.026096;0.153925;,
  0.952951;-0.241747;-0.182872;,
  0.532009;-0.023763;0.846405;,
  0.911030;-0.099676;0.400111;,
  -0.885128;-0.228642;-0.405304;,
  -0.220145;-0.432740;-0.874227;,
  0.971730;0.185463;0.146095;,
  0.967246;0.252233;0.028506;,
  0.444752;-0.049547;0.894282;,
  0.750274;-0.068340;0.657585;,
  -0.387560;-0.282647;-0.877444;,
  0.505214;-0.347897;-0.789763;,
  0.929118;0.239393;-0.281835;,
  0.751239;0.008550;-0.659975;,
  -0.927312;-0.187170;-0.324130;,
  0.942403;0.083014;0.324014;,
  0.443791;-0.349328;-0.825239;,
  0.867083;-0.197821;0.457202;,
  0.460949;-0.117590;0.879601;,
  0.936975;-0.317135;-0.146641;,
  0.187942;-0.113052;0.975652;,
  -0.977082;-0.076413;0.198676;,
  -0.991002;0.127117;-0.041905;,
  -0.986001;0.127305;0.107683;,
  -0.921339;-0.028003;0.387750;,
  -0.945145;0.048577;0.323020;,
  -0.958911;0.124642;0.254859;,
  -0.332014;-0.567614;-0.753380;;
  29;
  4;20,11,7,21;,
  4;22,5,3,23;,
  4;12,25,24,19;,
  4;26,1,0,27;,
  4;9,3,5,10;,
  4;6,8,2,0;,
  4;9,11,4,3;,
  4;6,0,1,7;,
  4;19,24,28,18;,
  4;23,3,4,29;,
  4;21,7,1,26;,
  4;27,0,2,30;,
  4;16,32,31,17;,
  4;6,7,11,9;,
  4;6,9,10,8;,
  4;29,4,11,20;,
  4;17,31,33,15;,
  4;13,34,32,16;,
  4;14,35,38,39;,
  4;18,28,35,14;,
  4;15,33,25,12;,
  4;8,15,12,2;,
  4;36,18,14,37;,
  4;37,14,39,40;,
  4;22,13,16,5;,
  4;10,17,15,8;,
  4;5,16,17,10;,
  4;41,19,18,41;,
  4;2,12,19,30;;
 }
 MeshTextureCoords {
  38;
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;;
 }
}