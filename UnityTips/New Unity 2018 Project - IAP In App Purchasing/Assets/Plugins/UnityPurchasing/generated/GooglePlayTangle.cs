#if UNITY_ANDROID || UNITY_IPHONE || UNITY_STANDALONE_OSX || UNITY_TVOS
// WARNING: Do not modify! Generated file.

namespace UnityEngine.Purchasing.Security {
    public class GooglePlayTangle
    {
        private static byte[] data = System.Convert.FromBase64String("ATb0x77Bh9naq0wiKKKuHEYXtIaYGxUaKpgbEBiYGxsasnpyY6wyuuKnQqe9iSZo0VLf0/3m/8jsb0KBWLektjbN38WRZI3XneKjChxTbJxCrBnTdzamalqaJtIGpv/9JIRrgQEWSKnR+jQfU37qlKaY+5CZ3jfhdZc4qt0oU5Fei2oYbbgfHN9U60KktLDfoqLGyCHXsfnURcvjVcYkjm+m5x2VP3xf6xmLLT0SlzH1jrLnB70UGTkKTGLD8/e+LnCZh53GFcAsJ561mdf2UFC4CmzCzGlchnJ7fe8ktMettdo8/+9CwuGKzpuCvu5ZKpgbOCoXHBMwnFKc7RcbGxsfGhlJq0ea9tVLtByDINnn0N3dfqxhc3dhieu/rSBjwRgZGxob");
        private static int[] order = new int[] { 2,1,11,6,8,7,8,8,10,9,13,13,13,13,14 };
        private static int key = 26;

        public static readonly bool IsPopulated = true;

        public static byte[] Data() {
        	if (IsPopulated == false)
        		return null;
            return Obfuscator.DeObfuscate(data, order, key);
        }
    }
}
#endif
