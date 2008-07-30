/*
 * MPQ Extractor ( SFmpq.dll wrapper )
 * Copyright (C) 2005-2007 WRS <thewrs@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

using System;
using System.IO;
using System.Collections;
using SFmpqapi;

namespace MPQE {
	class MPQE {
	
		private string version ="1.4";
		private int hMPQ = 0;
		private int hFile = 0;
		private int FileSize = 0;
		private int FileRead = 0;
		
		private bool option_usepatchfiles = false;
		//	private bool option_listonly = false;
		private bool option_verbose = false;
		private string option_outdir = "MPQOUT";
		private bool option_lowercase = false;
		private string option_baseMPQ;
		private string option_searchglob;

		private ArrayList extractedFiles = new ArrayList();

		[STAThread]
		static void Main(string[] args){
			MPQE mpqe = new MPQE();
			Console.Write("MPQ-Extractor v{0} by WRS", mpqe.version);
			try {
				Console.WriteLine(" powered by {0}", SFmpq.MpqGetVersionString());
			} catch (Exception e){
				Console.WriteLine();
				Console.WriteLine("Fatal: Could not locate SFmpq.dll");
				return;
			}
			Console.WriteLine();
			if (args.Length == 0){
				mpqe.helper();
				return;
			}
			for(int i = 0; i < args.Length; ++i){
				if (args[i].StartsWith("/")){
					switch(args[i]){
						case "/p":	// use patch files if available
							Console.WriteLine("Using patch MPQ archives if available: Enabled");
							mpqe.option_usepatchfiles = true;
							break;
						case "/l":
							Console.WriteLine("Lowercase output: Enabled");
							mpqe.option_lowercase = true;
							break;
						case "/v":
							Console.WriteLine("Verbose output: Enabled");
							mpqe.option_verbose = true;
							break;
						case "/d":
							if (i + 1 >= args.Length){
								Console.WriteLine("Fatal: No output directory specified for /d");
								mpqe.helper();
								return;
							};
							++i;
							Console.WriteLine("Output directory: {0}", args[i]);
							mpqe.option_outdir = args[i];
							break;
						default:
							Console.WriteLine("Fatal: Unknown option {0}", args[i]);
							mpqe.helper();
							return;
							break;
					};
				} else {
					if (mpqe.option_baseMPQ == null)
						mpqe.option_baseMPQ = args[i];
					else
						mpqe.option_searchglob = args[i];
				};
			}
			if (mpqe.option_baseMPQ == null){
				Console.WriteLine("Fatal: Did not provide MPQ file for extraction!");
				mpqe.helper();
				return;
			};
			mpqe.worker();
		}
		
		private void helper(){
			Console.WriteLine("Extracts compressed files from MoPAQ archives.");
			Console.WriteLine();
			Console.WriteLine("MPQE [options] <MPQfile> [glob]");
			Console.WriteLine();
			Console.WriteLine("Options:");
			Console.WriteLine(" /p\t\tExtract newer files from patch MPQ archives if available");
			Console.WriteLine(" /d <directory>\tSet output directory ( default: MPQOUT )");
			Console.WriteLine(" /v\t\tEnable verbose output");
			Console.WriteLine(" /l\t\tUse lowercase filenames");
		//	Console.WriteLine("\t /l\tExtract listfile ( dumped to console )");
		}
		
		private void worker(){
			if (!File.Exists(option_baseMPQ)){
				Console.WriteLine("Fatal: Could not locate MPQ archive {0}", option_baseMPQ);
				return;
			};
			FileInfo fi = new FileInfo(option_baseMPQ);
			if (option_usepatchfiles && fi.Name.ToLower().StartsWith("patch")){
				Console.WriteLine("Error: {0} is already a patch file, ignoring /p option", fi.Name);
				option_usepatchfiles = false;
			}
			if (option_usepatchfiles)
				foreach(string patchfile in Directory.GetFiles(fi.DirectoryName, "patch*.MPQ"))
					mpqExtract(patchfile);
			mpqExtract(option_baseMPQ);
		}
		
		private void mpqExtract(string fileMPQ){
			Console.WriteLine("Extracting from "+fileMPQ);
			if (SFmpq.SFileOpenArchive(fileMPQ, 0, 0, ref hMPQ) != 1){
				Console.WriteLine("Error: Could not open {0}", fileMPQ);
				return;
			};
			if (SFmpq.SFileOpenFileEx(hMPQ, "(listfile)", 0, ref hFile) != 1){
				SFmpq.SFileCloseArchive(hMPQ);
				Console.WriteLine("Error: Could not find (listfile) in "+fileMPQ);
				return;
			};
			byte[] buffer;
			FileSize = SFmpq.SFileGetFileSize(hFile, ref FileSize);
			buffer = new byte[FileSize];
			if (SFmpq.SFileReadFile(hFile, buffer, (uint)FileSize, ref FileRead, IntPtr.Zero) != 1){
				SFmpq.SFileCloseFile(hFile);
				SFmpq.SFileCloseArchive(hMPQ);
				Console.WriteLine("Error: Could not read (listfile) in "+fileMPQ);
				return;
			};
			SFmpq.SFileCloseFile(hFile);
			System.Text.ASCIIEncoding enc = new System.Text.ASCIIEncoding();
			string list = enc.GetString(buffer);
			string[] segs;
			string dirpath;
			foreach(string file in list.Split('\n')){
				if (file == "")
					return;
				if (this.extractedFiles.Contains(file)) continue;
				segs = file.Split('\\');
				dirpath = String.Join("\\", segs, 0, segs.Length - 1);
				if(option_lowercase)
					dirpath = dirpath.ToLower();
				if (this.option_searchglob != null && !Match(option_searchglob, file.Trim(), false)) continue;
				if (SFmpq.SFileOpenFileEx(hMPQ, file.Trim(), 0, ref hFile) != 1){
					Console.WriteLine("Error: Could not find "+file.Trim()+" in "+fileMPQ);
					continue;
				};
				FileSize = SFmpq.SFileGetFileSize(hFile, ref FileSize);
				if (FileSize == 0){
					if (option_verbose)
						Console.WriteLine("Skipping: {0} (NULL)", file.Trim());
					continue;
				}
				buffer = new byte[FileSize];
				if (SFmpq.SFileReadFile(hFile, buffer, (uint)FileSize, ref FileRead, IntPtr.Zero) != 1){
					SFmpq.SFileCloseFile(hFile);
					Console.WriteLine("Error: Could not read "+file.Trim()+" in "+fileMPQ);
					continue;
				};
				if (!Directory.Exists(option_outdir+Path.DirectorySeparatorChar+dirpath))
					Directory.CreateDirectory(option_outdir+Path.DirectorySeparatorChar+dirpath);
				FileStream fs;
				if (option_lowercase)
					fs = new FileStream(option_outdir+Path.DirectorySeparatorChar+file.Trim().ToLower(), FileMode.Create, FileAccess.Write);
				else
					fs = new FileStream(option_outdir+Path.DirectorySeparatorChar+file.Trim(), FileMode.Create, FileAccess.Write);
				fs.Write(buffer, 0, FileSize);
				fs.Flush();
				fs.Close();
				if (option_verbose)
					Console.WriteLine("Extracted: {0} ({1})", file.Trim(), bytes2text(FileSize));
				this.extractedFiles.Add(file);
			};
			SFmpq.SFileCloseArchive(hMPQ);
		}
		
		public bool Match(string pattern, string s, bool caseSensitive){
			char[] Wildcards = new char[]{'*', '?'};
			if (!caseSensitive){
				pattern = pattern.ToLower();
				s = s.ToLower();
			}
			if (pattern.IndexOfAny(Wildcards) == -1)
				return (s == pattern);
			int i = 0;
			int j = 0;
			while (i < s.Length && j < pattern.Length && pattern[j] != '*'){
				if ((pattern[j] != s[i]) && (pattern[j] != '?'))
					return false;
				i++;
				j++;
			}
			if (j == pattern.Length)
				return s.Length == pattern.Length;
		
			int cp = 0;
			int mp = 0;
			while(i < s.Length){
				if (j < pattern.Length && pattern[j] == '*'){
					if ((j++) >= pattern.Length)
						return true;
					mp = j;
					cp = i+1;
				} else if (j < pattern.Length && (pattern[j] == s[i] || pattern[j] == '?')){
					j++;
					i++;
				} else {
					j = mp;
					i = cp++;
				};
			}
		
			while (j < pattern.Length && pattern[j] == '*')
				j++;
			return j >= pattern.Length;
		}

		public string bytes2text(int bytes){	// not long?
			if (bytes < 1024) return bytes+"B";
			if (bytes < 1024 * 1024) return Math.Round((decimal)bytes / 1024, 0)+"K";
			if (bytes < 1024 * 1024 * 1024) return Math.Round((decimal)bytes / 1024 / 1024, 1)+"M";
			return "moo";
		}
	}
}
