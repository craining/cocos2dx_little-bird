package com.zgy.hellobirdcoco.view;


import android.app.Dialog;
import android.content.Context;
import android.content.DialogInterface;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup.LayoutParams;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.zgy.hellobirdcoco.R;
public class AppDialog extends Dialog {

	public AppDialog(Context context, int theme) {
		super(context, theme);
	}

	public AppDialog(Context context) {
		super(context);
	}

	public static class Builder {

		private Context context;
		private String title;
		private CharSequence message;
		private String positiveButtonText;
		private String negativeButtonText;
		private String neutralButtonText;
		private View contentView;
		private DialogInterface.OnClickListener positiveButtonClickListener, negativeButtonClickListener, neutralButtonClickListener;

		public Builder(Context context) {
			this.context = context;

		}

		public Builder setMessage(CharSequence message) {
			this.message = message;

			return this;

		}

		public Builder setMessage(int message) {
			this.message = (String) context.getText(message);

			return this;

		}

		public Builder setTitle(int title) {
			this.title = (String) context.getText(title);

			return this;

		}

		public Builder setTitle(String title) {
			this.title = title;

			return this;

		}

		public Builder setContentView(View v) {
			this.contentView = v;

			return this;

		}

		public Builder setPositiveButton(int positiveButtonText,

		DialogInterface.OnClickListener listener) {

			this.positiveButtonText = (String) context

			.getText(positiveButtonText);

			this.positiveButtonClickListener = listener;

			return this;

		}

		public Builder setPositiveButton(String positiveButtonText,

		DialogInterface.OnClickListener listener) {

			this.positiveButtonText = positiveButtonText;

			this.positiveButtonClickListener = listener;

			return this;

		}

		/**
		 * 
		 * @Description:初始化dialog中第二个按钮的文字和点击事件
		 * @param negativeButtonText
		 * @param listener
		 * @return
		 * @see: 
		 * @since: 
		 * @author: hanlx
		 * @date:2013-2-21
		 */
		public Builder setNegativeButton(int negativeButtonText,

		DialogInterface.OnClickListener listener) {
			this.negativeButtonText = (String) context.getText(negativeButtonText);
			this.negativeButtonClickListener = listener;

			return this;

		}

		public Builder setNegativeButton(String negativeButtonText,

		DialogInterface.OnClickListener listener) {
			this.negativeButtonText = negativeButtonText;
			this.negativeButtonClickListener = listener;

			return this;

		}
		
		/**
		 * 
		 * @Description:初始化dialog中第三个按钮的值和点击事件
		 * @param negativeButtonText
		 * @param listener
		 * @return
		 * @see: 
		 * @since: 
		 * @author: hanlx
		 * @date:2013-2-21
		 */
		public Builder setNeutralButton(int neutralButtonText, DialogInterface.OnClickListener listener) {
			this.neutralButtonText = (String)context.getText(neutralButtonText);
			this.neutralButtonClickListener = listener;
			return this;
		}
		
		public Builder setNeutralButton(String neutralButtonText, DialogInterface.OnClickListener listener){
			this.neutralButtonText = neutralButtonText;
			this.neutralButtonClickListener = listener;
			return this;
		}

		public AppDialog create() {
			LayoutInflater inflater = (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);

			final AppDialog dialog = new AppDialog(context, R.style.dialog);// R.style.dialog

			View layout = inflater.inflate(R.layout.view_dialog, null);
			dialog.addContentView(layout, new LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.WRAP_CONTENT));

			((TextView) layout.findViewById(R.id.title)).setText(title);

			if (positiveButtonText != null) {
				((Button) layout.findViewById(R.id.positiveButton)).setText(positiveButtonText);

				if (positiveButtonClickListener != null) {
					((Button) layout.findViewById(R.id.positiveButton)).setOnClickListener(new View.OnClickListener() {

						public void onClick(View v) {
							positiveButtonClickListener.onClick(dialog, DialogInterface.BUTTON_POSITIVE);
						}
					});
				}
			} else {
				layout.findViewById(R.id.positiveButton).setVisibility(View.GONE);

			}

			if (negativeButtonText != null) {
				((Button) layout.findViewById(R.id.negativeButton)).setText(negativeButtonText);

				if (negativeButtonClickListener != null) {
					((Button) layout.findViewById(R.id.negativeButton)).setOnClickListener(new View.OnClickListener() {

						public void onClick(View v) {
							negativeButtonClickListener.onClick(dialog, DialogInterface.BUTTON_NEGATIVE);

						}
					});
				}
			} else {
				layout.findViewById(R.id.negativeButton).setVisibility(View.GONE);
			}
			
			if(neutralButtonText != null){
				Button neutralButton = (Button) layout.findViewById(R.id.third_button);
				neutralButton.setText(neutralButtonText);
				neutralButton.setVisibility(View.VISIBLE);
				if(neutralButtonClickListener != null){
					neutralButton.setOnClickListener(new View.OnClickListener() {
						
						@Override
						public void onClick(View v) {
							// TODO Auto-generated method stub
							neutralButtonClickListener.onClick(dialog, DialogInterface.BUTTON_NEUTRAL);
						}
					});
				}
			}
			if (positiveButtonText == null && negativeButtonText == null && neutralButtonText == null) {
				layout.findViewById(R.id.positiveButtonLayout).setVisibility(View.GONE);
			}
			if (message != null) {
				((TextView) layout.findViewById(R.id.message)).setText(message);
			} else if (contentView != null) {
				((LinearLayout) layout.findViewById(R.id.content)).removeAllViews();
				((LinearLayout) layout.findViewById(R.id.content)).addView(contentView, new LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT));

			}
			dialog.setContentView(layout);
			return dialog;

		}

	}

}
