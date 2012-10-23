// shadow.frag



void rgb_to_hsv(vec4 rgb, out vec4 outcol)
{
	float cmax, cmin, h, s, v, cdelta;
	vec3 c;

	cmax = max(rgb[0], max(rgb[1], rgb[2]));
	cmin = min(rgb[0], min(rgb[1], rgb[2]));
	cdelta = cmax-cmin;

	v = cmax;
	if (cmax!=0.0)
		s = cdelta/cmax;
	else {
		s = 0.0;
		h = 0.0;
	}

	if (s == 0.0) {
		h = 0.0;
	}
	else {
		c = (vec3(cmax, cmax, cmax) - rgb.xyz)/cdelta;

		if (rgb.x==cmax) h = c[2] - c[1];
		else if (rgb.y==cmax) h = 2.0 + c[0] -  c[2];
		else h = 4.0 + c[1] - c[0];

		h /= 6.0;

		if (h<0.0)
			h += 1.0;
	}

	outcol = vec4(h, s, v, rgb.w);
}

void hsv_to_rgb(vec4 hsv, out vec4 outcol)
{
	float i, f, p, q, t, h, s, v;
	vec3 rgb;

	h = hsv[0];
	s = hsv[1];
	v = hsv[2];

	if(s==0.0) {
		rgb = vec3(v, v, v);
	}
	else {
		if(h==1.0)
			h = 0.0;
		
		h *= 6.0;
		i = floor(h);
		f = h - i;
		rgb = vec3(f, f, f);
		p = v*(1.0-s);
		q = v*(1.0-(s*f));
		t = v*(1.0-(s*(1.0-f)));
		
		if (i == 0.0) rgb = vec3(v, t, p);
		else if (i == 1.0) rgb = vec3(q, v, p);
		else if (i == 2.0) rgb = vec3(p, v, t);
		else if (i == 3.0) rgb = vec3(p, q, v);
		else if (i == 4.0) rgb = vec3(t, p, v);
		else rgb = vec3(v, p, q);
	}

	outcol = vec4(rgb, hsv.w);
}

 
void main (void)
{
	vec4 rgb, hsv;
	
	rgb_to_hsv(gl_Color, hsv);
	if(hsv[2] >= 0.4) hsv[2] = 1.0;
	else if(hsv[2] >= 0.3) hsv[2] = 0.5;
	else if(hsv[2] == 0.0) hsv[2] = 0.0;
	hsv_to_rgb(hsv, rgb);
	gl_FragColor = rgb;
	
/*	int step = 3;
	float astep = 1.0 / step;
	int ri = (int) (gl_Color.r * step);
	int gi = (int) (gl_Color.g * step);
	int bi = (int) (gl_Color.b * step);*/
//	gl_FragColor = gl_Color;
//	gl_FragColor = shadow + (gl_Color - shadow) * shadow2DProj(texture, gl_TexCoord[0]);
}
