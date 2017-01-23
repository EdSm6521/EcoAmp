using Android.Content;
using Android.Graphics;
using Android.Graphics.Drawables;
using Android.Graphics.Drawables.Shapes;
using Android.Views;
using System.Collections.Generic;

public class MyOvalShape : View
    {
        private readonly ShapeDrawable _shape;

        public MyOvalShape(Context context) : base(context)
        {

            var paint = new Paint();
            paint.SetARGB(255, 200, 255, 0);
            paint.SetStyle(Paint.Style.Stroke);
            paint.StrokeWidth = 4;

            _shape = new ShapeDrawable(new OvalShape());
            _shape.Paint.Set(paint);

            _shape.SetBounds(20, 20, 300, 200);

    }

       static float bp = 0;
       static List<float> ampValue = new List<float>();
       static bool empty = true;
        public void clearAmpValues()
        {
            empty = true;
            ampValue.Clear();
        }

        public void setAmpValue(float _value)
        {
            empty = false;
            ampValue.Add(_value);
            if (ampValue.Count > 80)
            {
                ampValue.RemoveAt(0);
            }
        }

        protected override void OnDraw(Canvas canvas)
        {
            bp++;
            var paint = new Paint();
            paint.SetARGB(255, 200, 255, 0);
            paint.SetStyle(Paint.Style.Stroke);
            paint.StrokeWidth = 4;
            Path pt = new Path();
            pt.MoveTo(0, (canvas.Height / 2));
            try
            {
                for (int r = 0; r < ampValue.Count; r++)
                {
                    if (empty) { return; }
                    float avalue = (canvas.Height / 2) - (ampValue[r]);
                    pt.LineTo(r * 5, avalue);
                }
            }
            catch (System.Exception _e) { }
            canvas.DrawPath(pt,paint);
            this.Invalidate();
            // ampValue.Clear();
        }

    }
