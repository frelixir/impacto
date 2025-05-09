#include "inst_movie.h"

#include "inst_macros.inc"

#include <math.h>

#include "expression.h"
#include "../game.h"
#include "../log.h"
#include "../mem.h"
#include "../profile/scriptvars.h"
#include "../profile/vm.h"
#include "../profile/game.h"
#include "../video/videosystem.h"
#include "interface/input.h"

namespace Impacto {

namespace Vm {

using namespace Impacto::Profile::ScriptVars;

VmInstruction(InstPlayMovie) {
  StartInstruction;
  PopUint8(playMode);
  if (playMode == 99) {  // PlayMovieRecover - used when loading a save file
    PopExpression(playModeEx);  // playMode of the video at save time
    PopExpression(playView);
    PopExpression(playNo);
    PopExpression(movCancelFlag);
    ImpLogSlow(LogLevel::Warning, LogChannel::VMStub,
               "STUB instruction PlayMovie(playMode: {:d}, playModeEx: {:d}, "
               "playView: {:d}, playNo: {:d}, movCancelFlag: {:d})\n",
               playMode, playModeEx, playView, playNo, movCancelFlag);

  } else {
    PopUint8(playView);
    PopExpression(playNo);
    PopExpression(movCancelFlag);
    if (Profile::GameFeatures & GameFeature::Video) {
      Io::Stream* stream;
      auto err = Io::VfsOpen("movie", playNo, &stream);
      if (err != IoError_OK) {
        ImpLog(LogLevel::Error, LogChannel::Video,
               "Failed to open movie for playback: IO error {:s}\n", err);
        return;
      }
      int flags = 0;
      if (playMode >= 8) {
        playMode -= 8;
        flags = 4;
      }
      if (playMode <= 5) {
        if (44 & (1 << playMode)) flags |= 8;
      }
      if (playMode == 3 || !playMode) flags |= 1;
      if (playMode == 5) flags |= 4;
      if ((playMode & 0xFFFFFFFD) == 0) flags = flags | 2;
      if (Video::Players[0]->IsPlaying) Video::Players[0]->Stop();
      Video::Players[0]->Play(stream, flags & 8, flags & 4);
      SetFlag(SF_MOVIEPLAY, true);
      SetFlag(SF_MOVIE_DRAWWAIT, true);
    }
    BlockThread;
    ImpLogSlow(LogLevel::Warning, LogChannel::VMStub,
               "STUB instruction PlayMovie(playMode: {:d}, playView: {:d}, "
               "playNo: {:d}, movCancelFlag: {:d})\n",
               playMode, playView, playNo, movCancelFlag);
  }
}
VmInstruction(InstPlayMovieOld) {
  StartInstruction;
  PopUint8(playMode);
  PopExpression(playNo);
  PopExpression(movCancelFlag);
  if (Profile::GameFeatures & GameFeature::Video) {
    Io::Stream* stream;
    Io::VfsOpen("movie", playNo, &stream);
    if (Video::Players[0]->IsPlaying) Video::Players[0]->Stop();
    Video::Players[0]->Play(stream, playMode == 5, playMode == 5);
  }
  BlockThread;
  ImpLogSlow(LogLevel::Warning, LogChannel::VMStub,
             "STUB instruction PlayMovie(playMode: {:d}, playNo: {:d}, "
             "movCancelFlag: {:d})\n",
             playMode, playNo, movCancelFlag);
}
VmInstruction(InstMovie) {
  StartInstruction;
  PopUint8(type);
  switch (type) {
    case 0: {  // Restart
      ImpLogSlow(LogLevel::Warning, LogChannel::VMStub,
                 "STUB instruction Movie(type: Restart)\n");
    } break;
    case 1: {  // Pause
      ImpLogSlow(LogLevel::Warning, LogChannel::VMStub,
                 "STUB instruction Movie(type: Pause)\n");
    } break;
  }
}
VmInstruction(InstMovieMain) {
  StartInstruction;
  PopUint8(type);
  switch (type) {
    case 0:
      if (Profile::GameFeatures & GameFeature::Video) {
        if ((Interface::PADinputButtonWentDown & Interface::PAD1A ||
             Interface::PADinputMouseWentDown & Interface::PAD1A) ||
            !Video::Players[0]->IsPlaying) {
          Video::Players[0]->Stop();
          SetFlag(SF_MOVIEPLAY, false);
          SetFlag(SF_MOVIE_DRAWWAIT, false);
        } else if (Video::Players[0]->IsPlaying) {
          ResetInstruction;
          BlockThread;
        }
      }
      break;
    case 1:
      thread->ScriptParam = GetFlag(SF_MOVIEPLAY);
      break;
    case 2:  // Stop
      if (Profile::GameFeatures & GameFeature::Video) {
        Video::Players[0]->Stop();
        SetFlag(SF_MOVIEPLAY, false);
        SetFlag(SF_MOVIE_DRAWWAIT, false);
      }
      break;
    case 3:  // StopWait
      if (Profile::GameFeatures & GameFeature::Video) {
        if ((Interface::PADinputButtonWentDown & Interface::PAD1A ||
             Interface::PADinputMouseWentDown & Interface::PAD1A) ||
            !Video::Players[0]->IsPlaying) {
          Video::Players[0]->Stop();
          ScrWork[SW_MOVIEFRAME] = 65535;
          SetFlag(SF_MOVIEPLAY, false);
          SetFlag(SF_MOVIE_DRAWWAIT, false);
        }
      }
      break;
  }
}
VmInstruction(InstLoadMovie) {
  StartInstruction;
  PopExpression(arg1);
  ImpLogSlow(LogLevel::Warning, LogChannel::VMStub,
             "STUB instruction LoadMovie(arg1: {:d})\n", arg1);
}
VmInstruction(InstPlayMovieMemory) {
  StartInstruction;
  PopUint8(playMode);
  if (playMode == 99) {
    PopExpression(playModeEx);
    PopExpression(playView);
    PopExpression(movCancelFlag);
    ImpLogSlow(LogLevel::Warning, LogChannel::VMStub,
               "STUB instruction PlayMovie(playMode: {:d}, playModeEx: {:d}, "
               "playView: {:d}, movCancelFlag: {:d})\n",
               playMode, playModeEx, playView, movCancelFlag);

  } else {
    PopUint8(playView);
    PopExpression(movCancelFlag);
    ImpLogSlow(LogLevel::Warning, LogChannel::VMStub,
               "STUB instruction PlayMovie(playMode: {:d}, playView: {:d}, "
               "movCancelFlag: {:d})\n",
               playMode, playView, movCancelFlag);
  }
}
VmInstruction(InstSFDpause) {
  StartInstruction;
  PopUint8(paused);
  ImpLogSlow(LogLevel::Warning, LogChannel::VMStub,
             "STUB instruction SFDpause(paused: {:d})\n", paused);
}

}  // namespace Vm

}  // namespace Impacto
